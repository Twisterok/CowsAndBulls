#ifndef __PACKET_MANAGER_H__
#define __PACKET_MANAGER_H__

// Own includes
#include <network/Packet.h>

// std includes
#include <memory>
#include <string>
#include <unordered_map>

// Qt includes

using namespace std;

class PacketFactory
{	
	void                    addGenerator(string key, std::function<Packet*()> generator);
	void                    removeGenerator(string key);
public:
	PacketFactory() {}
	virtual ~PacketFactory() {}

	unique_ptr<Packet>	CreatePacket(string packetName);	
protected:
	unordered_map<string, std::function<Packet*()> >   m_generators;
};

#endif
