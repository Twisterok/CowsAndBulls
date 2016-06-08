#ifndef __USER_H__
#define __USER_H__

// own includes
#include <utils/utils.h>

// std includes
#include <string>


using namespace std;

class User
{
public:

    User(string login, string pwd) :
        m_id(-1),
        m_login(login),
        m_pwd(pwd),
        m_nickname(login),
        m_wins(0),
        m_loses(0),
        m_isOnline(user_status::OFFLINE)
    { }
    int m_id;
    string  m_login;
    string  m_pwd;
    string  m_nickname;
    int m_wins;
    int m_loses;
    int m_isOnline;
};

#endif

