#ifndef __ERROR_HAPPEND_H__
#define __ERROR_HAPPEND_H__

// Own includes
#include <network/packet/Packet.h>

// std includes
#include <string>


using namespace std;

class  ErrorHappend : public Packet
{

public:
    void                    setErrorCode(int errCode);
    void                    setDescription(string description);
    virtual std::string     getTag()	override;
	void				    pack(std::string& data)	override;
	void				    unpack(const boost::property_tree::ptree& objectTree) override;
	std::unique_ptr<Packet> exec() override;
protected:
    int         m_errorCode;
    string      m_description;
};

#endif
