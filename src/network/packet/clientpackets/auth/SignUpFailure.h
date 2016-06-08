#ifndef __SIGNUP_FAILURE_H__
#define __SIGNUP_FAILURE_H__

// Own includes
#include <network/packet/clientpackets/ErrorHappend.h>

// std includes
#include <string>

class SignUpFailure : public ErrorHappend
{
public:
    virtual std::string     getTag()	override;
};

#endif
