#ifndef __PACKET_H__
#define __PACKET_H__
//own includes
#include <network/session/Session.h>

// std includes
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <memory>

// qt includes

class Packet
{
public:
	virtual ~Packet() {}    
    virtual std::string			    getTag() = 0;
    virtual void				    pack(std::string& data) = 0;
    virtual void				    unpack(const boost::property_tree::ptree& objectTree) = 0;
    virtual std::unique_ptr<Packet> exec()  {return nullptr;}
    virtual void                    SetSessionObject(std::shared_ptr<Session>) {}
protected:
    virtual void verifyArgs() {}
};

#endif
