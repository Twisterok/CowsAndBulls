#ifndef __PACKET_H__
#define __PACKET_H__
//own includes

// std includes
#include <string>
#include <vector>

// qt includes
class Packet
{
public:
    virtual std::string getTag() = 0;
    virtual void        pack(std::vector<unsigned char>& data) = 0;
    virtual void        unpack(const std::vector<unsigned char>& data) = 0;
    virtual void        exec() = 0;
};

#endif
