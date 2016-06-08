#ifndef __GET_NEXT_CSN_RESPONSE_H__
#define __GET_NEXT_CSN_RESPONSE_H__

// Own includes
#include <network/packet/Packet.h>

// std includes
#include <string>
#include <stdint.h>


using namespace std;

class  GetNextCSNResponse : public Packet
{
public:
    /*  setters */
    void                    setClientId(int errCode);
    void                    setValue(int64_t value);
    
    /*  getters */
    int                     getClientId()   { return m_clientId; }
    int64_t                 getValue()      { return m_value; }
    

	std::string			    getTag()	override;
	void				    pack(std::string& data)	override;
	void				    unpack(const boost::property_tree::ptree& objectTree) override;

protected:
    int     m_clientId;
    int64_t m_value;
};

#endif
