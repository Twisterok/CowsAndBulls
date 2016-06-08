#include <network/connection/tcp/tcp_connection.h>
#include <utils/Exception.h>
#include <utils/ErrorCodes.h>

tcp_connection::~tcp_connection()
{
	disconnect();    
}
void tcp_connection::connect(string host,int port)
{
	m_host = host;
	m_port = port;
    boost::asio::ip::tcp::endpoint ep( boost::asio::ip::address::from_string(host), port);
    try
    {	    
	    m_sock.connect(ep);
    }
    catch (boost::system::system_error& ex)
    {
        /*  rethrow */
        throw Exception(errors::ER_CONNECTION_LOST, "Can not initialize connection to server. Description: "+std::string(ex.what()));
    }
}
void tcp_connection::write(std::vector<unsigned char>& data)
{
    /*----------------------------------------------------------*  
    *   Exceptions:                                             *
    *   boost::system::system_error. Thrown on failure.         *
    *   An error code of boost::asio::error::eof                *
    *   indicates that the connection was closed by the peer.   *
    *-----------------------------------------------------------*/
    try
    {
	    m_sock.write_some(boost::asio::buffer(data));
    }
    catch (boost::system::system_error& ex)
    {
        /*  rethrow */
        throw Exception(errors::ER_CONNECTION_LOST, "Connection to server has been lost. Description: "+std::string(ex.what()));
    }
}
int tcp_connection::read(std::vector<unsigned char>& data)
{
    /*----------------------------------------------------------*  
    *   Exceptions:                                             *
    *   boost::system::system_error. Thrown on failure.         *
    *   An error code of boost::asio::error::eof                *
    *   indicates that the connection was closed by the peer.   *
    *-----------------------------------------------------------*/
    try
    {
        return m_sock.read_some(boost::asio::buffer(data));
    }
    catch (boost::system::system_error& ex)
    {
        /*  rethrow */
        throw Exception(errors::ER_CONNECTION_LOST, "Connection to server has been lost. Description: "+std::string(ex.what()));
    }	
}
void tcp_connection::reconnect()
{
	if (m_host.empty())	throw Exception(errors::ER_NETWORK_ERROR,"Error: ip is not set");
	if (m_port <= 0)	throw Exception(errors::ER_NETWORK_ERROR,"Error: port is not set");

	disconnect();
	connect(m_host,m_port);
}
void tcp_connection::disconnect()
{
	m_sock.close();
}

int tcp_connection::available()
{
    return m_sock.available();
}

bool tcp_connection::is_open()
{

    return m_sock.is_open();
}
