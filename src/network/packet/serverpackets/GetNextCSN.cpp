// Own includes
#include <stdint.h>
#include <network/packet/serverpackets/GetNextCSN.h>
#include <network/packet/clientpackets/ErrorHappend.h>
#include <network/packet/clientpackets/GetNextCSNResponse.h>

#include <utils/Exception.h>
#include <utils/ErrorCodes.h>


std::string	GetNextCSN::getTag()
{
	return "GetNextCSN";
}
void	GetNextCSN::pack(std::string& data)
{
	stringstream    outStream;
	boost::property_tree::ptree objectTree;

	objectTree.put("packet_name", getTag());
    objectTree.put("params.client_id", m_clientId);

	boost::property_tree::write_json(outStream, objectTree);
	data = outStream.str();
}
void	GetNextCSN::unpack(const boost::property_tree::ptree& objectTree)
{
	m_clientId	=   objectTree.get<int>("params.client_id");
}

void GetNextCSN::setClientId(int clientId)
{
    m_clientId = clientId;
}
