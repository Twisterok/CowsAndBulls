#ifndef __SIGNUP_SUCCESS_H__
#define __SIGNUP_SUCCESS_H__

// Own includes
#include <network/packet/Packet.h>

// std includes
#include <string>

using namespace std;

class SignUpSuccess : public Packet
{
public:
    void                setLogin(string login);
    void                setPwd(string pwd);

    string              getLogin()      {return m_login;}
    string              getPwd()        {return m_pwd;}

    std::string			getTag()	override;
    void				pack(std::string& data)	override;
    void				unpack(const boost::property_tree::ptree& objectTree) override;

protected:
    std::string m_login;
    std::string m_pwd;
};

#endif
