#include <network/packet/clientpackets/auth/SignInSuccess.h>


std::string			SignInSuccess::getTag()
{
    return "SignInSuccess";
}

void    SignInSuccess::pack(std::string& data)
{
    stringstream    outStream;
    boost::property_tree::ptree objectTree;

    objectTree.put("packet_name", getTag());
    objectTree.put("params.uid", m_id);
    objectTree.put("params.login", m_login);
    objectTree.put("params.pwd", m_pwd);
    objectTree.put("params.nickname", m_nickname);
    objectTree.put("params.wins", m_wins);
    objectTree.put("params.loses", m_loses);
    objectTree.put("params.is_online", m_isOnline);

    boost::property_tree::write_json(outStream, objectTree);
    data = outStream.str();
}

void    SignInSuccess::unpack(const boost::property_tree::ptree& objectTree)
{
    m_id        =	objectTree.get<int>("params.uid");
    m_login     =	objectTree.get<std::string>("params.login");
    m_pwd       =	objectTree.get<std::string>("params.pwd");
    m_nickname  =	objectTree.get<std::string>("params.nickname");
    m_wins      =	objectTree.get<int>("params.wins");
    m_loses     =	objectTree.get<int>("params.loses");
    m_isOnline  =   objectTree.get<int>("params.is_online");
}

void    SignInSuccess::setLogin(string login)
{
    m_login = login;
}

void    SignInSuccess::setPwd(string pwd)
{
    m_pwd = pwd;
}

void    SignInSuccess::setNickname(string nickname)
{
    m_nickname = nickname;
}

void    SignInSuccess::setId(int id)
{
    m_id = id;
}

void    SignInSuccess::setWins(int wins)
{
    m_wins = wins;
}

void    SignInSuccess::setLoses(int loses)
{
    m_loses = loses;
}

void    SignInSuccess::setIsOnline(int online)
{
    m_isOnline = online;
}
