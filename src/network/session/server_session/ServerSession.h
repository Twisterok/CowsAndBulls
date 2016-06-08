#ifndef __SERVER_SESSION_H__
#define __SERVER_SESSION_H__
// std includes
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/detail/endian.hpp>

// Own includes
// Own includes
#include <context/CBServerContext.h>
#include <utils/Exception.h>
#include <utils/ErrorCodes.h>
#include <network/session/Session.h>
#include <database/mysql/command.h>

// Qt incldues
#include <QObject>

using boost::asio::ip::tcp;

class ServerSession   : public Session
{
    Q_OBJECT
public:
    ServerSession(boost::asio::io_service& io_service);
    ~ServerSession();

    std::shared_ptr<command>&   getDB()   {return m_dbCon;}
    void                        setDB(command* dbCon);
protected:

    void handle_read_content(const boost::system::error_code& error);
    std::shared_ptr<command>    m_dbCon;

signals:
    void    NewMessage(std::string);
};

#endif
