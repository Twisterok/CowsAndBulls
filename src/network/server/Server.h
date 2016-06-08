#ifndef __SERVER_H__
#define __SERVER_H__
#include <network/session/server_session/ServerSession.h>
#include <QObject>

class Server : public QObject
{
    Q_OBJECT
public:
  Server(boost::asio::io_service& io_service, short port);

private:
    void do_accept();
    void handle_accept(std::shared_ptr<ServerSession> session,
          const boost::system::error_code& error);

    boost::asio::io_service& io_service;
    tcp::acceptor acceptor_;
signals:
    void NewMessage(std::string);
private slots:
    void slotNewThreadMessage(std::string);
};

#endif
