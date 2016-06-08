#ifndef __PACKET_PROCESSOR_H__
#define __PACKET_PROCESSOR_H__
// own includes
#include <network/packet/Packet.h>
#include <network/packet/factory/PacketFactory.h>
#include <network/packet/PacketProcessor.h>
#include <utils/ErrorCodes.h>
#include <utils/Exception.h>

// std includes
#include <string>
#include <sstream>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/detail/endian.hpp>

// Qt includes

using namespace std;

namespace PacketProcessor
{

    //-------------------------------------------------------------------
    void FillMessage(unique_ptr<Packet>& packet, vector<unsigned char>& message);
    short DecodeHeader(const std::vector<unsigned char>& message);
    unique_ptr<Packet> DecodePacket(shared_ptr<PacketFactory>& packetFactory,const std::vector<unsigned char>& message);
    //-------------------------------------------------------------------
    template <class ConnectionType>
    void write(shared_ptr<ConnectionType>& con, unique_ptr<Packet> packet)
    {
        vector<unsigned char> message;
        FillMessage(std::move(packet),message);
        con->write(message);
    }
//-------------------------------------------------------------------
    template <class ConnectionType>
    unique_ptr<Packet> read(shared_ptr<ConnectionType>& con, shared_ptr<PacketFactory>& packetFactory)
    {
        string				    response;
        vector<unsigned char>   message;
        unique_ptr<Packet>      result;        

        /*	read length */
        message.resize(2);
        con->read(message);

        short   cmdLen = DecodeHeader(message);

        /*	Read message	*/
        message.resize(cmdLen);
        con->read(message);

        auto packet = DecodePacket(packetFactory,message);
        return std::move(packet);
    }
}

#endif
