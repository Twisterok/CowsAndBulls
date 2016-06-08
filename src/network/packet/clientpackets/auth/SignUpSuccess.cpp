#include <network/packet/clientpackets/auth/SignUpSuccess.h>


std::string			SignUpSuccess::getTag()
{
    return "SignUpSuccess";
}

void    SignUpSuccess::pack(std::string& data)
{
    stringstream    outStream;
    boost::property_tree::ptree objectTree;

    objectTree.put("packet_name", getTag());
    objectTree.put("params.login", m_login);
    objectTree.put("params.pwd", m_pwd);

    boost::property_tree::write_json(outStream, objectTree);
    data = outStream.str();
}

void    SignUpSuccess::unpack(const boost::property_tree::ptree& objectTree)
{
    m_login     =	objectTree.get<std::string>("params.login");
    m_pwd       =	objectTree.get<std::string>("params.pwd");
}

void    SignUpSuccess::setLogin(string login)
{
    m_login = login;
}

void    SignUpSuccess::setPwd(string pwd)
{
    m_pwd = pwd;
}
