#ifndef __GCHELPER_EXCEPTION__
#define __GCHELPER_EXCEPTION__
#include <exception>
#include <string>

using namespace std;
class Exception: public exception
{
	string			m_errDescription;
	unsigned int	m_errCode;
public:
	Exception(unsigned int err,string description) : exception(), m_errCode(err), m_errDescription(description) {}
	virtual const char* what() const throw()
	{
	    return m_errDescription.c_str();
	}

	unsigned int getErrCode()	{ return m_errCode; }
};
#endif