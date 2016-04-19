#ifndef __PACKET_HANDLER_H__
#define __PACKET_HANDLER_H__

// std includes
#include <string>

// Qt includes

class PacketHandler
{
public:
    static void processPacket(std::string& data);
};

#endif
