// Own includes
#include <network/packet/serverpackets/auth/decorator/SignUpDecorator.h>
#include <network/packet/clientpackets/auth/SignUpFailure.h>
#include <network/packet/clientpackets/auth/SignUpSuccess.h>
#include <database/DAO/UsersDAO.h>
#include <utils/utils.h>

std::unique_ptr<Packet> SignUpDecorator::exec()
{
    try
    {
        if (m_pwd.compare(m_confirmPwd) != 0)
        {
            throw Exception(errors::ER_PWDS_NOT_MATCH,"Passwords don't match");
        }
        User usr(m_login,m_pwd);
        UsersDAO::AddUser(m_session->getDB(),usr);

        unique_ptr<SignUpSuccess>   response(new SignUpSuccess);
        response->setLogin(usr.m_login);
        response->setPwd(usr.m_pwd);
        return std::move(response);
    }
    catch (Exception& ex)
    {
        unique_ptr<SignUpFailure>   response(new SignUpFailure);
        response->setDescription(ex.what());
        response->setErrorCode(ex.getErrCode());
        return std::move(response);
    }
    catch (std::exception& ex)
    {
        unique_ptr<SignUpFailure>   response(new SignUpFailure);
        response->setDescription(ex.what());
        response->setErrorCode(errors::ER_INTERNAL_ERROR);
        return std::move(response);
    }
}

void    SignUpDecorator::SetSessionObject(std::shared_ptr<Session> ptr)
{
    m_session = std::dynamic_pointer_cast<ServerSession>(ptr);
    if (!m_session) throw Exception(errors::ER_INTERNAL_ERROR, "Session object invalid");
}
