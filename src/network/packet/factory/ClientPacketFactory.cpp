#include <network/packet/factory/ClientPacketFactory.h>
#include <network/packet/clientpackets/ErrorHappend.h>
#include <network/packet/clientpackets/GetNextCSNResponse.h>

//----------------------------------------------------------
ClientPacketFactory::ClientPacketFactory()
{
    /*  add some packets here   */
    this->addGenerator("ErrorHappend",      [](){return new ErrorHappend();});
    this->addGenerator("GetNextCSNResponse",[](){return new GetNextCSNResponse();});
}
