#ifndef __SIGN_UP_DECORATOR_H__
#define __SIGN_UP_DECORATOR_H__

// Own includes
#include <network/session/server_session/ServerSession.h>
#include <network/packet/Packet.h>
#include <network/packet/serverpackets/auth/SignUp.h>
#include <database/mysql/command.h>

// std includes
#include <string>
#include <memory>


using namespace std;

class  SignUpDecorator : public SignUp
{    
public:
    std::unique_ptr<Packet> exec() override;
    virtual void            SetSessionObject(std::shared_ptr<Session>) override;

private:

    std::shared_ptr<ServerSession>  m_session;
};

#endif
