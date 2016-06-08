#ifndef __GET_NEXT_CSN_H__
#define __GET_NEXT_CSN_H__

// Own includes
#include <network/packet/Packet.h>

// std includes
#include <string>


using namespace std;

class  GetNextCSN : public Packet
{    
public:
    void                    setClientId(int clientId);
	std::string			    getTag()	override;
	void				    pack(std::string& data)	override;
	void				    unpack(const boost::property_tree::ptree& objectTree) override;
protected:
    int m_clientId;
};

#endif
