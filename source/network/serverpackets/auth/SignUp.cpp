// Own includes
#include <network/serverpackets/auth/SignUp.h>


std::string	SignUp::getTag()
{
	return "SignUp";
}
void	SignUp::pack(std::string& data)
{
	stringstream    outStream;
	boost::property_tree::ptree objectTree;

	objectTree.put("packet_name", getTag());
	objectTree.put("params.login", m_login);
	objectTree.put("params.pwd", m_pwd);
	objectTree.put("params.confirm_pwd", m_confirmPwd);

	boost::property_tree::write_json(outStream, objectTree);
	data = outStream.str();
}
void	SignUp::unpack(const boost::property_tree::ptree& objectTree)
{
	m_login	=		objectTree.get<std::string>("params.login");
	m_pwd	=		objectTree.get<std::string>("params.pwd");
	m_confirmPwd =	objectTree.get<std::string>("params.confirm_pwd");
}
void	SignUp::exec()
{
	
}