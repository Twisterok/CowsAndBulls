#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__
#include <network/connection/connection.h>
#include <boost/asio.hpp>

class tcp_connection : public connection
{
	boost::asio::io_service			m_io_svc;
	boost::asio::ip::tcp::socket	m_sock;
	boost::asio::ip::tcp::resolver	m_resolver;
public:
	tcp_connection() : m_sock(m_io_svc), m_resolver(m_io_svc)
	{
		m_host = "";
		m_port = 0;
	}
	~tcp_connection();
	void	connect(string host,int port)		        override;
	void	write(std::vector<unsigned char>& data)		override;
	int		read(std::vector<unsigned char>& data)		override;
    int     available()									override;
    bool    is_open()                                   override;
	void	reconnect()									override;
	void	disconnect()								override;
};
#endif
