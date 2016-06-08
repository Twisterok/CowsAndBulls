#ifndef __SIGNOUT_FAILURE_H__
#define __SIGNOUT_FAILURE_H__

// Own includes
#include <network/packet/clientpackets/ErrorHappend.h>

// std includes
#include <string>

class SignOutFailure : public ErrorHappend
{
public:
    virtual std::string     getTag()	override;
};

#endif
