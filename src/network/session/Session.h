#ifndef __SESSION_H__
#define __SESSION_H__

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

// Qt incldues
#include <QObject>

using boost::asio::ip::tcp;

class Session   : public QObject,
        public std::enable_shared_from_this<Session>
{
    Q_OBJECT
public:
    Session(boost::asio::io_service& io_service);
    virtual ~Session();
    void   start();

    tcp::socket& socket()   { return socket_; }
protected:

    void readHeader();
    void handle_read_header(const boost::system::error_code& error);
    virtual void handle_read_content(const boost::system::error_code& error) = 0;
    void handle_write(const boost::system::error_code& error);

    tcp::socket                 socket_;
    std::vector<unsigned char>  data;
};

#endif
