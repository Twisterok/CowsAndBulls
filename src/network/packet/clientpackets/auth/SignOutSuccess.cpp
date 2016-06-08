#include <network/packet/clientpackets/auth/SignOutSuccess.h>

std::string			SignOutSuccess::getTag()
{
    return "SignOutSuccess";
}

void    SignOutSuccess::pack(std::string& data)
{
    stringstream    outStream;
    boost::property_tree::ptree objectTree;

    objectTree.put("packet_name", getTag());

    boost::property_tree::write_json(outStream, objectTree);
    data = outStream.str();
}

void    SignOutSuccess::unpack(const boost::property_tree::ptree&)   {   return; }
