#ifndef __SIGN_OUT_H__
#define __SIGN_OUT_H__

// Own includes
#include <network/Packet.h>
#include <network/session/Session.h>

// std includes
#include <string>


using namespace std;

class  SignOut : public Packet
{
public:
    void    setUID(int uid);
    int     getUID()      {return m_uid;}

    std::string		getTag()	override;
    void            pack(std::string& data)	override;
    void            unpack(const boost::property_tree::ptree& objectTree) override;
    virtual void    SetSessionObject(std::shared_ptr<Session>) {}
protected:
    int             m_uid;
};

#endif
