// Own includes
#include <network/packet/serverpackets/auth/SignOut.h>
#include <utils/Exception.h>
#include <utils/ErrorCodes.h>
#include <utils/encryption/CrcCrypto.h>

std::string	SignOut::getTag()
{
    return "SignOut";
}
void	SignOut::pack(std::string& data)
{
	stringstream    outStream;
	boost::property_tree::ptree objectTree;

    objectTree.put("packet_name", getTag());
    objectTree.put("params.uid", m_uid);

	boost::property_tree::write_json(outStream, objectTree);
	data = outStream.str();
}
void	SignOut::unpack(const boost::property_tree::ptree& objectTree)
{
    m_uid	=		objectTree.get<int>("params.uid");
}

void    SignOut::setUID(int uid)
{
    m_uid = uid;
}
