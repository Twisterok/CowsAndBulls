#ifndef __SIGN_UP_H__
#define __SIGN_UP_H__

// Own includes
#include <network/Packet.h>
#include <network/session/Session.h>
// std includes
#include <string>


using namespace std;

class  SignUp : public Packet
{
public:
    void                setLogin(string login);
    void                setPwd(string pwd);
    void                setConfirmPwd(string confirmPwd);

    string              getLogin()      {return m_login;}
    string              getPwd()        {return m_pwd;}
    string              getConfirmPwd() {return m_confirmPwd;}

    std::string			getTag()	override;
    void				pack(std::string& data)	override;
    void				unpack(const boost::property_tree::ptree& objectTree) override;
    virtual void                    SetSessionObject(std::shared_ptr<Session>) {}
protected:
    string m_login;
    string m_pwd;
    string m_confirmPwd;
};

#endif
