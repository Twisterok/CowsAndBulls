#ifndef __ODG_SERVER_CONTEXT_H__
#define __ODG_SERVER_CONTEXT_H__
// Own includes
#include <network/connection/tcp/tcp_connection.h>
#include <network/packet/factory/PacketFactory.h>
#include <network/connection/factory/ConnectionFactory.h>
#include <utils/definitions.h>
#include <database/mysql/command.h>

// std includes
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>


class CBServerContext
{
    CBServerContext();
    ~CBServerContext();

    std::shared_ptr<PacketFactory>                              m_packetFactory;
    ODBCParameters                                              m_DBParams;        

    bool                                                        m_DBLog;
    bool                                                        m_ExtraLog;
    int                                                         m_ODGPort;
public:	

    std::shared_ptr<PacketFactory>  GetPacketFactory();
    ODBCParameters  GetDBParams()   {return m_DBParams;}
    bool            DBLogON()       {return m_DBLog; }
    bool            ExtraLogON()    {return m_ExtraLog; }
    int             GetODGPort()    {return m_ODGPort; }
    static  CBServerContext*	singleton();
};

#endif
