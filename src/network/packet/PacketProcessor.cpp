// Own includes
#include <network/packet/PacketProcessor.h>
#include <utils/ErrorCodes.h>
#include <utils/Exception.h>

// std includes
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/detail/endian.hpp>
// Qt includes

namespace PacketProcessor
{
    void FillMessage(unique_ptr<Packet>& packet, vector<unsigned char>& message)
    {
        message.clear();
        std::string zipPacket;
        packet->pack(zipPacket);

        short   cmdLen = zipPacket.size();

        /*  Cast to Big Endian  */
    #if defined BOOST_LITTLE_ENDIAN
        message.push_back((reinterpret_cast<unsigned char*>(&cmdLen))[1]);
        message.push_back((reinterpret_cast<unsigned char*>(&cmdLen))[0]);
    #elif defined BOOST_BIG_ENDIAN
        message.push_back((reinterpret_cast<unsigned char*>(&cmdLen))[0]);
        message.push_back((reinterpret_cast<unsigned char*>(&cmdLen))[1]);
    #else
        throw Exception(errors::ER_INTERNAL_ERROR,"Unknown byte order");
    #endif
        message.insert(message.end(),zipPacket.begin(),zipPacket.end());
    }

    short DecodeHeader(const std::vector<unsigned char>& message)
    {
        short   cmdLen = 0;

        /*	Construct back to system byte order	*/
#if defined BOOST_LITTLE_ENDIAN
        reinterpret_cast<unsigned char*>(&cmdLen)[0] = message[1];
        reinterpret_cast<unsigned char*>(&cmdLen)[1] = message[0];
#elif defined BOOST_BIG_ENDIAN
        reinterpret_cast<unsigned char*>(&cmdLen)[0] = message[0];
        reinterpret_cast<unsigned char*>(&cmdLen)[1] = message[1];
#else
        throw Exception(errors::ER_INTERNAL_ERROR,"Unknown byte order");
#endif

        return cmdLen;
    }


    unique_ptr<Packet> DecodePacket(shared_ptr<PacketFactory>& packetFactory,const std::vector<unsigned char>& message)
    {
        string				    response;
        response.assign(message.begin(),message.end());
        boost::property_tree::ptree objectTree;
        stringstream strStream;
        strStream << response;

        boost::property_tree::read_json(strStream, objectTree);
        string packetName = objectTree.get<std::string>("packet_name");
        auto receivedPacket = packetFactory->CreatePacket(packetName);
        receivedPacket->unpack(objectTree);

        return std::move(receivedPacket);
    }
}
