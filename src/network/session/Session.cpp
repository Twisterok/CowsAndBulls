#include <network/session/Session.h>
#include <network/packet/PacketProcessor.h>

using boost::asio::ip::tcp;

Session::Session(boost::asio::io_service& io_service)
    : socket_(io_service)
{
}
 
Session::~Session()
{
}

void Session::start()
{
    readHeader();
}

void Session::handle_read_header(const boost::system::error_code& error)
{
    try
    {
        if (!error)
        {
            short   cmdLen = PacketProcessor::DecodeHeader(data);

            data.clear();
            data.resize(cmdLen);

            boost::asio::async_read(socket_,
                boost::asio::buffer(data),
                boost::bind(&Session::handle_read_content, shared_from_this(),
                boost::asio::placeholders::error));
        }
    }
    catch(...)  { /* Connection will be closed    */}
}

void Session::handle_write(const boost::system::error_code& error)
{
    if (!error)     readHeader();
}

void Session::readHeader()
{
    data.clear();
    data.resize(2);
    boost::asio::async_read(socket_,
        boost::asio::buffer(data),
        boost::bind(
          &Session::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));
}
