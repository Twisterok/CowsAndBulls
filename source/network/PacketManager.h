#ifndef __PACKET_MANAGER_H__
#define __PACKET_MANAGER_H__

// Own includes
#include <network/Packet.h>

// std includes
#include <memory>
#include <string>

// Qt includes


class PacketManager
{
    void                    addPacket(std::string key, shared_ptr<Packet>& pPacket);
    void                    removePacket(std::string key);
public:

    PacketManager() {}
    std::shared_ptr<Packet> getPacket(std::string commandName);

protected:
    std::unordered_map<std::string, std::shared_ptr<Packet> >   m_packets;
};

#endif
