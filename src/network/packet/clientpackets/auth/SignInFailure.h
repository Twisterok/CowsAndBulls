#ifndef __SIGNIN_FAILURE_H__
#define __SIGNIN_FAILURE_H__

// Own includes
#include <network/packet/clientpackets/ErrorHappend.h>

// std includes
#include <string>

class SignInFailure : public ErrorHappend
{
public:
    virtual std::string     getTag()	override;
};

#endif
