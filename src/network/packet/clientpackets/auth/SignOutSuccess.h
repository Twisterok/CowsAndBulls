#ifndef __SIGNOUT_SUCCESS_H__
#define __SIGNOUT_SUCCESS_H__

// Own includes
#include <network/packet/Packet.h>

// std includes
#include <string>

using namespace std;

class SignOutSuccess : public Packet
{
public:

    std::string     getTag()	override;
    void			pack(std::string& data)	override;
    void			unpack(const boost::property_tree::ptree& objectTree) override;
protected:
};

#endif
