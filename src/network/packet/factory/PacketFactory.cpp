// Own includes
#include <network/packet/factory/PacketFactory.h>

// std includes
#include <stdexcept>
#include <boost/algorithm/string.hpp>


std::unique_ptr<Packet> PacketFactory::CreatePacket(string name)
{
	std::unique_ptr<Packet> result;
    boost::to_lower(name);
    auto it = m_generators.find(name);
    if (it == m_generators.end())	throw std::runtime_error("Packet [ "+name+" ] isn't supported");

	auto MakeNewPacket = it->second;
	result.reset(MakeNewPacket());
	return std::move(result);
}
//----------------------------------------------------------
void	PacketFactory::addGenerator(std::string key, std::function<Packet*()> generator)
{
    auto insertResult = m_generators.insert(std::make_pair(boost::to_lower_copy(key), generator));
    if (!insertResult.second)	throw runtime_error("Generator tag ["+ key +"] already exists. Adding is unavailable");
}
//----------------------------------------------------------
void	PacketFactory::removeGenerator(std::string key)
{
    auto it = m_generators.find(boost::to_lower_copy(key));
    if (it == m_generators.end())	return;

	m_generators.erase(it);
}
