#ifndef __QT_UTILS_H__
#define __QT_UTILS_H__
#include <QTextCodec>
#include <QNetworkInterface>
#include <string>
#include <boost/algorithm/string.hpp>

/*------------------------------------------------------------------------*
 * QT �������� ������ � unicode.
 *
 * �������, �������� ��������� �� �������,
 * ��� ����������� �� �� ����� ����� �������� �� � unicode
 *
 * � IDE ������� ����� �� ��������� System
 * (�����������->���������->��������� ��������->��������� ������->System)
 *
 *-----------------------------------------------------------------------*/
class CodepageProcessor
{
    QTextCodec* m_codec;
    CodepageProcessor()
    {
        m_codec = QTextCodec::codecForName("cp1251");
    }

public:

    QString toUnicode(std::string str)
    {
        return m_codec->toUnicode(str.c_str(),str.size());
    }

    std::string fromUnicode(QString str)
    {
        return m_codec->fromUnicode(str.data(),str.size()).toStdString();
    }

    static CodepageProcessor*  singleton()
    {
        static CodepageProcessor s_context;
        return &s_context;
    }
};



namespace utils
{
    bool isLocalHost(std::string ip);
}

#endif // QT_UTILS

