// Own includes
#include <network/PacketHandler.h>
#include <context/GlobalContext.h>

// std includes
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// Qt includes

void PacketHandler::processPacket(std::string& data)
{
	boost::property_tree::ptree objectTree;
	stringstream strStream;
	strStream << data;
	
    boost::property_tree::read_json(strStream, objectTree);
	
	string packetName = objectTree.get<std::string>("PacketName");

	auto pGlobalCtx = GlobalContext::singleton();
	auto packet = pGlobalCtx->m_factory->CreatePacket(packetName);
	
	packet->unpack(objectTree);
	packet->exec();
}