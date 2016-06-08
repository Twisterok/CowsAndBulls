#ifndef __SIGNIN_SUCCESS_H__
#define __SIGNIN_SUCCESS_H__

// Own includes
#include <network/packet/Packet.h>

// std includes
#include <string>

using namespace std;

class SignInSuccess : public Packet
{
public:

    void                setId(int id);
    void                setLogin(string login);
    void                setPwd(string pwd);
    void                setNickname(string nickname);
    void                setWins(int wins);
    void                setLoses(int loses);
    void                setIsOnline(int online);

    int                 getId()         {return m_id;}
    string              getLogin()      {return m_login;}
    string              getPwd()        {return m_pwd;}
    string              getNickname()   {return m_nickname;}
    int                 getWins()       {return m_wins;}
    int                 getLoses()      {return m_loses;}
    int                 isOnline()      {return m_isOnline;}

    std::string			getTag()	override;
    void			pack(std::string& data)	override;
    void			unpack(const boost::property_tree::ptree& objectTree) override;

protected:
    int m_id;
    std::string m_login;
    std::string m_pwd;
    std::string m_nickname;
    int m_wins;
    int m_loses;
    int m_isOnline;
};

#endif
