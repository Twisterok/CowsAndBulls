#ifndef __USERS_DAO_H__
#define __USERS_DAO_H__

// Own includes
#include <database/mysql/command.h>
#include <objects/User.h>

// std includes
#include <memory>
#include <string>

using namespace std;

class UsersDAO
{    
public:
    static bool UserExists(shared_ptr<command> dbCon,string login);
    static void GetUser(shared_ptr<command> dbCon,User& user);
    static void AddUser(shared_ptr<command> dbCon,User& user);
    static void UpdateUser(shared_ptr<command> dbCon,const User& user);
    static void DeleteUser(shared_ptr<command> dbCon,const User& user);
};

#endif
