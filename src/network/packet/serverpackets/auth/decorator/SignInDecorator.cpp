// Own includes
#include <network/packet/serverpackets/auth/decorator/SignInDecorator.h>
#include <network/packet/clientpackets/auth/SignInFailure.h>
#include <network/packet/clientpackets/auth/SignInSuccess.h>
#include <database/DAO/UsersDAO.h>
#include <utils/utils.h>

std::unique_ptr<Packet> SignInDecorator::exec()
{
    unique_ptr<Packet> responsePacket;
    try
    {
        User usr(m_login,"");
        UsersDAO::GetUser(m_session->getDB(),usr);

        if (usr.m_isOnline)                 throw Exception(errors::ER_USER_IS_ALREADY_ONLINE,
                                                            "User ["+m_login+"] is already online");
        if (usr.m_pwd.compare(m_pwd) !=0)   throw Exception(errors::ER_WRONG_PWD,"Wrong password");

        usr.m_isOnline = user_status::ONLINE;

        UsersDAO::UpdateUser(m_session->getDB(),usr);
        unique_ptr<SignInSuccess>   response(new SignInSuccess);
        response->setId(usr.m_id);
        response->setLogin(usr.m_login);
        response->setPwd(usr.m_pwd);
        response->setNickname(usr.m_nickname);
        response->setWins(usr.m_wins);
        response->setLoses(usr.m_loses);
        response->setIsOnline(usr.m_isOnline);
        return std::move(response);
    }
    catch (Exception& ex)
    {
        unique_ptr<SignInFailure>   response(new SignInFailure);
        response->setDescription(ex.what());
        response->setErrorCode(ex.getErrCode());
        return std::move(response);
    }
    catch (std::exception& ex)
    {
        unique_ptr<SignInFailure>   response(new SignInFailure);
        response->setDescription(ex.what());
        response->setErrorCode(errors::ER_INTERNAL_ERROR);
        return std::move(response);
    }
}

void    SignInDecorator::SetSessionObject(std::shared_ptr<Session> ptr)
{
    m_session = std::dynamic_pointer_cast<ServerSession>(ptr);
    if (!m_session) throw Exception(errors::ER_INTERNAL_ERROR, "Session object invalid");
}
