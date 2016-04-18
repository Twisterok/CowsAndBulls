#include <network/PacketManager.h>
#include <stdexcept>
#include <boost/algorithm/string.hpp>


std::shared_ptr<Packet> PacketManager::getPacket(std::string name)
{
    boost::to_upper(name);
    auto it = m_packets.find(name);
    if (it == m_packets.end())	throw std::runtime_error("Packet [ "+name+" ] isn't supported");

    return it->second;
}
//----------------------------------------------------------
void	PacketManager::addPacket(std::string key, shared_ptr<Packet>& pPacket)
{
    auto insertResult = m_packets.insert(std::make_pair(boost::to_upper_copy(key),pPacket));
    if (!insertResult.second)	throw Exception(errors::ER_INTERNAL_ERROR, "Packet tag ["+pPacket->getTag()+"] already exists. Adding is unavailable");
}
//----------------------------------------------------------
void	PacketManager::removePacket(std::string key)
{
    auto it = m_packets.find(boost::to_upper_copy(key));
    if (it == m_packets.end())	return;

    m_packets.erase(it);
}
