#ifndef __ODG_SERVER_H__
#define __ODG_SERVER_H__

// own includes
#include <network/server/Server.h>

// std includes
#include <memory>
#include <string>

// Qt includes
#include <QThread>


class ODGServerThread : public QThread
{
    Q_OBJECT

public:
    ODGServerThread(QObject* parent);
    ~ODGServerThread();
    void    setPort(int port);
private:

    int                     m_port;
    std::unique_ptr<Server> m_server;
    boost::asio::io_service io_service;
signals:
    void    NewLogMessage(std::string message);
    void    ServerError(std::string errDescription);
private slots:
    void    slotNewConnection();
    void    slotThreadNewMessage(std::string message);
protected:
    void    run() override;
};

#endif
