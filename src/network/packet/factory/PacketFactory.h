#ifndef __PACKET_FACTORY_H__
#define __PACKET_FACTORY_H__

// Own includes
#include <network/packet/Packet.h>

// std includes
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

// Qt includes

using namespace std;

class PacketFactory
{	
public:
	PacketFactory() {}
	virtual ~PacketFactory() {}

    unique_ptr<Packet>      CreatePacket(string packetName);
protected:
	void                    addGenerator(string key, std::function<Packet*()> generator);
	void                    removeGenerator(string key);
    std::unordered_map<string, std::function<Packet*()> >   m_generators;
};

#endif
