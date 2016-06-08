#include <network/session/server_session/ServerSession.h>
#include <network/packet/PacketProcessor.h>


using boost::asio::ip::tcp;

ServerSession::ServerSession(boost::asio::io_service &io_service) :
        Session(io_service)
{
}
ServerSession::~ServerSession()
{
}

void ServerSession::handle_read_content(const boost::system::error_code& error)
{
    try
    {
        if (!error)
        {
            /*  Receiving packet body   */
            auto receivedPacket = PacketProcessor::DecodePacket(CBServerContext::singleton()->GetPacketFactory(),data);
            emit NewMessage("Received packet ["+receivedPacket->getTag()+"]");

            /*  Process packet  */
            emit NewMessage("Processing packet ["+receivedPacket->getTag()+"]");
            receivedPacket->SetSessionObject(shared_from_this());
            auto responsePacket = receivedPacket->exec();

            /*  Serialize packet    */
            PacketProcessor::FillMessage(responsePacket,data);

            emit NewMessage("Sending response packet ["+responsePacket->getTag()+"]");
            boost::asio::async_write(socket_,
                boost::asio::buffer(data),
                boost::bind(&ServerSession::handle_write, shared_from_this(),
                  boost::asio::placeholders::error));
        }
    }
    catch(...)  { /* Connection will be closed    */}
}

void ServerSession::setDB(command* dbCon)
{
    m_dbCon.reset(dbCon);
}
