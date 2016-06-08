// Own includes
#include <context/CBServerContext.h>
#include <utils/definitions.h>
#include <utils/registry/RegistryHelper.h>
#include <network/packet/factory/ServerPacketFactory.h>
//#include <network/connection/factory/ServerConnectionFactory.h>

// std includes
// Qt includes

CBServerContext::CBServerContext() : m_DBLog(true),m_ExtraLog(false), m_ODGPort(0)
{    
    string				registry_string;
    registry_string.resize(1024,0);

    RegistryHelper regHelper;
    regHelper.openKey(HKEY_LOCAL_MACHINE,globals::CB_REGISTRY_PATH.c_str());

    regHelper.getKeyValueOptional("host",registry_string);
    if (!registry_string.empty())   m_DBParams.m_host = registry_string;
    else                            throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, globals::CB_REGISTRY_PATH +"\\host not found");

    regHelper.getKeyValueOptional("port",registry_string);
    if (!registry_string.empty())   m_DBParams.m_port = stoi(registry_string);
    else                            throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, globals::CB_REGISTRY_PATH +"\\port not found");

    regHelper.getKeyValueOptional("database",registry_string);
    if (!registry_string.empty())   m_DBParams.m_database = registry_string;
    else                            throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, globals::CB_REGISTRY_PATH +"\\database not found");

    regHelper.getKeyValueOptional("uid",registry_string);
    if (!registry_string.empty())   m_DBParams.m_uid = registry_string;
    else                            throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, globals::CB_REGISTRY_PATH +"\\uid not found");

    regHelper.getKeyValueOptional("pwd",registry_string);
    if (!registry_string.empty())   m_DBParams.m_pwd = registry_string;
    else                            throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, globals::CB_REGISTRY_PATH +"\\pwd not found");

    regHelper.getKeyValueOptional("DB Log",registry_string);
    if (strcmp(registry_string.c_str(),"OFF")==0)   m_DBLog = false;

    regHelper.getKeyValueOptional("Extra Log",registry_string);
    if (strcmp(registry_string.c_str(),"ON")==0)    m_ExtraLog = true;

    regHelper.getKeyValueOptional("CB Port",registry_string);
    if (!registry_string.empty())   m_ODGPort = stoi(registry_string);
    else                            throw Exception(errors::ER_REGISTRY_OPERATION_ERROR, globals::CB_REGISTRY_PATH +"\\ODG Port not found");

    m_packetFactory     = shared_ptr<PacketFactory>(new ServerPacketFactory);
}

CBServerContext::~CBServerContext()
{
}

std::shared_ptr<PacketFactory> CBServerContext::GetPacketFactory()
{
    return m_packetFactory;
}

CBServerContext*	CBServerContext::singleton()
{
    static CBServerContext s_context;
	return &s_context;
}
