#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T22:25:09
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CBServer
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

INCLUDEPATH += ..\..\source \
    C:\boost_1_55_0

TEMPLATE = app


HEADERS += \
    ..\..\source\context\GlobalContext.h \
    ..\..\source\network\ClientPacketFactory.h \
    ..\..\source\network\Packet.h \
    ..\..\source\network\PacketFactory.h \
    ..\..\source\network\PacketHandler.h \
    ..\..\source\network\ServerPacketFactory.h \
    ..\..\source\network\serverpackets\auth\SignUp.h \
    ..\..\source\network\clientpackets\auth\SignUpSuccess.h \
    ..\..\source\network\clientpackets\auth\SignUpFailed.h \
    ..\..\source\utils\ErrorCodes.h \
    ..\..\source\utils\Exception.h

SOURCES += \
    ..\..\source\context\GlobalContext.cpp \
    ..\..\source\network\ClientPacketFactory.cpp \
    ..\..\source\network\Packet.cpp \
    ..\..\source\network\PacketFactory.cpp \
    ..\..\source\network\PacketHandler.cpp \
    ..\..\source\network\ServerPacketFactory.cpp \
    ..\..\source\network\serverpackets\auth\SignUp.cpp \
    ..\..\source\network\clientpackets\auth\SignUpSuccess.cpp \
    ..\..\source\network\clientpackets\auth\SignUpFailed.cpp \
    main.cpp
