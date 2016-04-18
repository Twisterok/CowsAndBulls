#ifndef __SERVER_PACKET_MANAGER_H__
#define __SERVER_PACKET_MANAGER_H__

// Own includes
#include <network/Packet.h>

// std includes
#include <memory>
#include <string>
#include <unordered_map>

// Qt includes



class ServerPacketManager : public PacketManager
{   
public:
    ServerPacketManager();
};

#endif
