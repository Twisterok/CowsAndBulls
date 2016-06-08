#ifndef __DEFINITIONS_H__
#define	__DEFINITIONS_H__
#include <string>
#include <stdint.h>

#define STRINGIFY(str)			#str					
#define USERS_TABLE             "USERS_TMP"
namespace	logging_type
{
    const int   OFF			= 0;
	const int   ON			= 1;
	const int   ADVANCED	= 2;
	const int   FULL		= 3;
};

namespace user_status
{
    const   int         OFFLINE                  = 0;
    const   int         ONLINE                   = 1;
}

namespace globals
{
    const	std::string CB_REGISTRY_PATH        = "SOFTWARE\\NovaCard\\CB";
    const   int         BYTE_BITS_COUNT          = 8;
    const   int         MAX_THREADS              = 150;
    const   int64_t     BATCH_ID_MASK            = 0xFFFF000000;
    const   int64_t     CSN_MASK                 = 0x0000FFFFFF;
}

struct ODBCParameters
{
    std::string	m_host;
    int         m_port;
    std::string m_database;
    std::string m_uid;
    std::string m_pwd;

    void operator=(const ODBCParameters& param)
    {
        this->m_host        = param.m_host;
        this->m_port        = param.m_port;
        this->m_database    = param.m_database;
        this->m_uid         = param.m_uid;
        this->m_pwd         = param.m_pwd;
    }

    bool CheckParams()  {return !m_pwd.empty()&&
                                !m_host.empty()  &&
                                m_port > 0    &&
                                !m_uid.empty()    &&
                                !m_database.empty();}
};

struct ServerParams
{
    std::string	m_host;
    int         m_port;   

    void operator=(const ODBCParameters& param)
    {
        this->m_host        = param.m_host;
        this->m_port        = param.m_port;
    }

    bool CheckParams()  {return !m_host.empty()  && m_port > 0;  }
};

namespace events
{
    const int	EVENT_ODG_FIRST                 = 157000;
    //---------------------------------------------------
    const int	EVENT_ODG_SERVER_APP_START      = 157001;
    const int	EVENT_ODG_SERVER_START_LISTEN   = 157002;
    const int	EVENT_ODG_SERVER_PAUSE_LISTEN   = 157003;
    const int	EVENT_ODG_SERVER_STOP           = 157004;
    const int	EVENT_ODG_SERVER_APP_FINISHED   = 157005;
    //---------------------------------------------------
    const int	EVENT_ODG_LAST                  = 157999;
}

#endif
