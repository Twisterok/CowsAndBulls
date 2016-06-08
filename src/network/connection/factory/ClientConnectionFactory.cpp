// own include 

#include <network/connection/factory/ClientConnectionFactory.h>
#include <network/connection/tcp/tcp_connection.h>
#include <utils/ErrorCodes.h>
#include <utils/Exception.h>

// std includes
#include <string>

// Qt includes

shared_ptr<tcp_connection>  ClientConnectionFactory::MakeConnection(const string& host,const int& port)
{
    shared_ptr<tcp_connection> newConnection;
    newConnection = shared_ptr<tcp_connection>(new tcp_connection);
    newConnection->connect(host, port);
    return newConnection;
}
