#ifndef __SERVER_PACKET_MANAGER_H__
#define __SERVER_PACKET_MANAGER_H__

// Own includes
#include <network/packet/factory/PacketFactory.h>
#include <network/session/server_session/ServerSession.h>
// std includes

// Qt includes


class ServerPacketFactory : public PacketFactory
{   
public:
	ServerPacketFactory();   
private:
};

#endif
