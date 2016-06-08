#ifndef __CLIENT_CONNECTION_FACTORY_H__
#define __CLIENT_CONNECTION_FACTORY_H__
// own includes
#include <network/connection/factory/ConnectionFactory.h>

// std includes
#include <string>

// Qt includes

class ClientConnectionFactory : public ConnectionFactory
{
public:
    shared_ptr<tcp_connection>  MakeConnection(const string& host,const int& port) override;
};

#endif
