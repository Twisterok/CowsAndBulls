// Own includes
#include <network/packet/clientpackets/ErrorHappend.h>
#include <utils/definitions.h>
#include <utils/Exception.h>
#include <utils/ErrorCodes.h>


std::string	ErrorHappend::getTag()
{
    return "ErrorHappend";
}
void	ErrorHappend::pack(std::string& data)
{
	stringstream    outStream;
	boost::property_tree::ptree objectTree;

	objectTree.put("packet_name", getTag());
    objectTree.put("params.error_code", m_errorCode);
    objectTree.put("params.description", m_description);

	boost::property_tree::write_json(outStream, objectTree);
	data = outStream.str();
}
void	ErrorHappend::unpack(const boost::property_tree::ptree& objectTree)
{
	m_errorCode	    =   objectTree.get<int>("params.error_code");
    m_description	=   objectTree.get<string>("params.description");
}

void ErrorHappend::setErrorCode(int errCode)
{
    m_errorCode = errCode;
}

void ErrorHappend::setDescription(string description)
{
    m_description = description;
}

std::unique_ptr<Packet> ErrorHappend::exec()
{
    throw Exception(m_errorCode,m_description);
}
