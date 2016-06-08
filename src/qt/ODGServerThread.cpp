#include <qt/ODGServerThread.h>
#include <context/ODGServerContext.h>
#include <utils/definitions.h>
#include <utils/DBLogger.h>
#include <utils/ErrorCodes.h>
#include <utils/Exception.h>

#include <memory>

ODGServerThread::ODGServerThread(QObject* parent) : m_port(0),
                                        QThread(parent)
{
}

ODGServerThread::~ODGServerThread()
{    
    DBLogger::singleton()->LD_LogEvent(events::EVENT_ODG_SERVER_STOP,"ODG. Завершение работы сервера",0);
    io_service.stop();
    m_server.reset();
    DBLogger::singleton()->LD_LogEvent(events::EVENT_ODG_SERVER_STOP,"ODG. Завершение активных потоков",0);
}

void    ODGServerThread::run()
{
    try
    {
        if (!m_server)
        {
            m_server.reset(new Server(io_service,m_port));
            connect( (Server*)  m_server.get(),SIGNAL(NewMessage(std::string)),
                     this, SLOT(slotThreadNewMessage(std::string)));
        }
        emit NewLogMessage("Online Data Generator has started!");
        DBLogger::singleton()->LD_LogEvent(events::EVENT_ODG_SERVER_START_LISTEN,"ODG. Начало работы сервера",0);
        io_service.run();
    }
    catch (std::exception& ex)
    {
        emit ServerError("Server error: " + std::string(ex.what()));
    }
}

void    ODGServerThread::setPort(int port)
{
    if (isRunning())    throw Exception(errors::ER_NETWORK_ERROR, "Can't set port. Server is running.");
    m_port = port;
}

void    ODGServerThread::slotNewConnection()
{    
    emit NewLogMessage("New connection!");
}

void    ODGServerThread::slotThreadNewMessage(std::string message)
{
    emit NewLogMessage(message);
}
