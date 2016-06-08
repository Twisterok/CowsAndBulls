#ifndef __SIGN_IN_DECORATOR_H__
#define __SIGN_IN_DECORATOR_H__

#include <network/session/server_session/ServerSession.h>
#include <network/packet/Packet.h>
#include <network/packet/serverpackets/auth/SignIn.h>
#include <database/mysql/command.h>

// std includes
#include <string>
#include <memory>


using namespace std;


class  SignInDecorator : public SignIn
{	
public:
    std::unique_ptr<Packet> exec() override;
    virtual void            SetSessionObject(std::shared_ptr<Session>) override;

private:

    std::shared_ptr<ServerSession>  m_session;
};

#endif
