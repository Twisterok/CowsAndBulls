#ifndef __CONNECTION_FACTORY_H__
#define __CONNECTION_FACTORY_H__

//own includes 
#include <network/connection/tcp/tcp_connection.h>

// std includes
#include <string>
#include <memory>


// Qt includes

using namespace std;

class ConnectionFactory
{
public:
    virtual ~ConnectionFactory() {}

    virtual shared_ptr<tcp_connection>  MakeConnection(const string& host,const int& port) = 0;
};

#endif
