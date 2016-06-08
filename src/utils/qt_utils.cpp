#include <utils/qt_utils.h>

namespace utils
{
    bool isLocalHost(std::string ip)
    {
        if (boost::iequals(ip,"localhost")) return true;

        foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
        {
            if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                    ip.compare(address.toString().toStdString()) == 0)
            {
                return true;
            }
        }
        return false;
    }
}
