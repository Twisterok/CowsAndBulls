#ifndef __PACKET_H__
#define __PACKET_H__
//own includes

// std includes
#include <string>
#include <boost/property_tree/ptree.hpp>

// qt includes
class Packet
{
public:
    virtual std::string			getTag() = 0;
    virtual void				pack(std::string& data) = 0;
    virtual void				unpack(const boost::property_tree::ptree& objectTree) = 0;
    virtual void				exec() = 0;
};

#endif
