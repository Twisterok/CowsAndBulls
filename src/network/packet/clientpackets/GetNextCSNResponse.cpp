// Own includes
#include <network/packet/clientpackets/GetNextCSNResponse.h>
#include <utils/definitions.h>
#include <utils/Exception.h>
#include <utils/ErrorCodes.h>


std::string	GetNextCSNResponse::getTag()
{
    return "GetNextCSNResponse";
}
void	GetNextCSNResponse::pack(std::string& data)
{
	stringstream    outStream;
	boost::property_tree::ptree objectTree;

	objectTree.put("packet_name", getTag());
    objectTree.put("params.client_id", m_clientId);
    objectTree.put("params.value", m_value);

	boost::property_tree::write_json(outStream, objectTree);
	data = outStream.str();
}
void	GetNextCSNResponse::unpack(const boost::property_tree::ptree& objectTree)
{
	m_clientId  =   objectTree.get<int>("params.client_id");
    m_value	    =   objectTree.get<int64_t>("params.value");
}

void GetNextCSNResponse::setClientId(int clientId)
{
    m_clientId = clientId;
}

void GetNextCSNResponse::setValue(int64_t value)
{
    m_value = value;
}
