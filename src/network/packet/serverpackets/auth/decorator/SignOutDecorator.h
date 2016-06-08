#ifndef __SIGN_OUT_DECORATOR_H__
#define __SIGN_OUT_DECORATOR_H__

#include <network/session/server_session/ServerSession.h>
#include <network/packet/Packet.h>
#include <network/packet/serverpackets/auth/SignOut.h>
#include <database/mysql/command.h>

// std includes
#include <string>
#include <memory>


using namespace std;


class  SignOutDecorator : public SignOut
{
public:
    std::unique_ptr<Packet> exec() override;
    virtual void            SetSessionObject(std::shared_ptr<Session>) override;

private:

    std::shared_ptr<ServerSession>  m_session;
};

#endif
