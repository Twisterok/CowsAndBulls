#ifndef __PACKET_HANDLER_H__
#define __PACKET_HANDLER_H__
#include <vector>
#include <network/PacketManager.h>

class PacketHandler
{
public:
    static void processPacket(std::vector<unsigned char>& data);
};

#endif
