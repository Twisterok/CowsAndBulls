#include <database/DAO/UsersDAO.h>


bool UsersDAO::UserExists(shared_ptr<command> dbCon,string login)
{
    string sql = "select id from "+string(USERS_TABLE)+" where login = ?";

    std::vector<db_variant> params;
    std::vector<db_variant> row;
    params.push_back(login);
    dbCon->exec(sql,params);
    if (dbCon->fetch(row))  return true;
    return false;
}

void UsersDAO::GetUser(shared_ptr<command> dbCon,User& user)
{
    string sql = "select id,login, pwd,nickname,wins,loses, is_online from "+string(USERS_TABLE)+" where ";
    sql += user.m_id > 0 ? " id = ?" : " login = ?";

    std::vector<db_variant> params;
    std::vector<db_variant> row;
    if (user.m_id > 0)  params.push_back(user.m_id);
    else                params.push_back(user.m_login);

    dbCon->exec(sql,params);
    if (!dbCon->fetch(row))  throw Exception(errors::ER_USER_DOESNT_EXIST,"User ["+user.m_login+"] doesnt exist");

    user.m_id       = boost::get<int>(row[0]);
    user.m_login    = boost::get<string>(row[1]);
    user.m_pwd      = boost::get<string>(row[2]);
    user.m_nickname = boost::get<string>(row[3]);
    user.m_wins     = boost::get<int>(row[4]);
    user.m_loses    = boost::get<int>(row[5]);
    user.m_isOnline = boost::get<int>(row[6]);
}

void UsersDAO::AddUser(shared_ptr<command> dbCon,User& user)
{
    if (UserExists(dbCon,user.m_login)) throw Exception(errors::ER_USER_EXISTS,"User ["+user.m_login+"]");
    string sqlInsert = "insert into "+string(USERS_TABLE)+" (login, pwd, nickname, wins, loses, is_online)"
                 "values(?,?,?,?,?,?);";
    std::vector<db_variant> params;
    std::vector<db_variant> row;
    params.push_back(user.m_login);
    params.push_back(user.m_pwd);
    params.push_back(user.m_login);
    params.push_back(user.m_wins);
    params.push_back(user.m_loses);
    params.push_back(user.m_isOnline);
    dbCon->exec(sqlInsert,params);

    /*  Last Insert ID return unsigned long long    */
    dbCon->exec("SELECT LAST_INSERT_ID();");
    if (!dbCon->fetch(row)) throw Exception(errors::ER_MYSQL_ERROR,"User wasn't added");

    user.m_id = boost::get<int64_t>(row[0]);
}

void UsersDAO::UpdateUser(shared_ptr<command> dbCon,const User& user)
{
    if (!UserExists(dbCon,user.m_login)) throw Exception(errors::ER_USER_DOESNT_EXIST,"User ["+user.m_login+"] doesn't exist");
    string sqlInsert = "update "+string(USERS_TABLE)+" set "
                       " login = ?, "
                       " pwd = ?, "
                       " nickname = ?, "
                       " wins = ?, "
                       " loses = ?, "
                       " is_online = ? "
                       " where id = ?";
    std::vector<db_variant> params;
    params.push_back(user.m_login);
    params.push_back(user.m_pwd);
    params.push_back(user.m_nickname);
    params.push_back(user.m_wins);
    params.push_back(user.m_loses);
    params.push_back(user.m_isOnline);
    params.push_back(user.m_id);
    dbCon->exec(sqlInsert,params);
}
