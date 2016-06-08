// Own includes
#include <network/packet/serverpackets/auth/decorator/SignOutDecorator.h>
#include <network/packet/clientpackets/auth/SignOutFailure.h>
#include <network/packet/clientpackets/auth/SignOutSuccess.h>
#include <database/DAO/UsersDAO.h>
#include <utils/utils.h>

std::unique_ptr<Packet> SignOutDecorator::exec()
{
    unique_ptr<Packet> responsePacket;
    try
    {
        User usr("","");
        usr.m_id = m_uid;
        UsersDAO::GetUser(m_session->getDB(),usr);

        if (!usr.m_isOnline)                throw Exception(errors::ER_USER_IS_ALREADY_OFFLINE,
                                                            "User ["+usr.m_login+"] is offline");

        usr.m_isOnline = user_status::OFFLINE;

        UsersDAO::UpdateUser(m_session->getDB(),usr);
        unique_ptr<SignOutSuccess>   response(new SignOutSuccess);
        return std::move(response);
    }
    catch (Exception& ex)
    {
        unique_ptr<SignOutFailure>   response(new SignOutFailure);
        response->setDescription(ex.what());
        response->setErrorCode(ex.getErrCode());
        return std::move(response);
    }
    catch (std::exception& ex)
    {
        unique_ptr<SignOutFailure>   response(new SignOutFailure);
        response->setDescription(ex.what());
        response->setErrorCode(errors::ER_INTERNAL_ERROR);
        return std::move(response);
    }
}

void    SignOutDecorator::SetSessionObject(std::shared_ptr<Session> ptr)
{
    m_session = std::dynamic_pointer_cast<ServerSession>(ptr);
    if (!m_session) throw Exception(errors::ER_INTERNAL_ERROR, "Session object invalid");
}
