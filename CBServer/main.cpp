#include <QCoreApplication>
#include <context/CBServerContext.h>
#include <network/server/Server.h>
#include <network/session/server_session/ServerSession.h>
#include <network/packet/serverpackets/auth/decorator/SignUpDecorator.h>
#include <network/packet/serverpackets/auth/decorator/SignInDecorator.h>
#include <network/packet/serverpackets/auth/decorator/SignOutDecorator.h>

int main(int argc, char *argv[])
{    
    boost::asio::io_service			m_io_svc;
    try
    {
    std::unique_ptr<SignUp> signUpPacket(new SignUpDecorator);
    std::shared_ptr<ServerSession> sess(new ServerSession(m_io_svc));

    auto ctx = CBServerContext::singleton();
    unique_ptr<command> cmd(new command(ctx->GetDBParams().m_host,
                                        ctx->GetDBParams().m_port,
                                        ctx->GetDBParams().m_database,
                                        ctx->GetDBParams().m_uid,
                                        ctx->GetDBParams().m_pwd));
    sess->setDB(cmd.release());
    signUpPacket->setLogin("ASHOT_ONESHOT");
    signUpPacket->setPwd("1234");
    signUpPacket->setConfirmPwd("1234");
    signUpPacket->SetSessionObject(sess);
    std::unique_ptr<Packet> responsePacket = signUpPacket->exec();


    std::unique_ptr<SignIn> signInPacket(new SignInDecorator);
    signInPacket->setLogin("ASHOT_ONESHOT");
    signInPacket->setPwd("1234");
    signInPacket->SetSessionObject(sess);
    responsePacket = signInPacket->exec();

    std::unique_ptr<SignOut> signOutPacket(new SignOutDecorator);
    signOutPacket->setUID(13);
    signOutPacket->SetSessionObject(sess);
    responsePacket = signOutPacket->exec();
    int a = 0;

    }
    catch(Exception& ex)
    {
        string what = ex.what();
        return -1;
    }
    catch (std::exception& ex)
    {
        string what = ex.what();
        return -1;
    }

    return 0;
}
