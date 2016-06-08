#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <vector>
#include <string>

using namespace std;
class connection
{
public:
    virtual ~connection() {}
    virtual void	connect(string host,int port)       = 0;
	virtual void	write(std::vector<unsigned char>& data)		= 0;
	virtual int		read(std::vector<unsigned char>& data)		= 0;
	virtual	void	reconnect()									= 0;
	virtual	void	disconnect()								= 0;
    virtual int     available()									= 0;
    virtual bool    is_open()									= 0;

    virtual int     getDescriptor()    {return 0;}
	/*	Getters	*/
	string			getHost()	{	return m_host;	}
	int				getPort()	{	return m_port;	}
protected:
	string	m_host;
	int		m_port;
};
#endif
