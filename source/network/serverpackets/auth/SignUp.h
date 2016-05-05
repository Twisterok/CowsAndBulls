#ifndef __SIGN_UP_H__
#define __SIGN_UP_H__

// Own includes
#include <network/Packet.h>

// std includes
#include <string>


using namespace std;

class  SignUp : public Packet
{
	string m_login;
	string m_pwd;
	string m_confirmPwd;
public:
    void                setLogin(string login);
    void                setPwd(string pwd);
    void                setConfirmPwd(string confirmPwd);
	std::string			getTag()	override;
	void				pack(std::string& data)	override;
	void				unpack(const boost::property_tree::ptree& objectTree) override;
	void				exec() override;
};

#endif
