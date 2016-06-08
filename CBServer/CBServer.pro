#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T22:25:09
#
#-------------------------------------------------

QT       += core gui network

TARGET = CBServer
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += . ../src ./src $$(BOOST) $$quote($$(ProgramFiles)/MySQL/MySQL Connector C 6.1/include)

LIBS += -L$$quote($$(BOOST)\\stage\\lib\\)

LIBS += -lWtsapi32 \
        -lsecur32 \
        -ladvapi32 \
        -lversion

HEADERS += \
        ../src/context/CBServerContext.h \
        ../src/objects/User.h \
        ../src/database/DAO/UsersDAO.h \
        ../src/database/mysql/command.h \
        ../src/database/mysql/bind_result.hpp \
        ../src/database/mysql/bind_result_arr.hpp \
        ../src/database/mysql/bind_result_factory.hpp \
        ../src/database/mysql/bind_result_value.hpp \
        ../src/database/mysql/column.hpp \
        ../src/database/mysql/db_variant.h \
        ../src/database/mysql/mysql_lib.hpp \
        ../src/network/packet/Packet.h \
        ../src/network/packet/PacketProcessor.h \
        ../src/network/packet/clientPackets/ErrorHappend.h \
        ../src/network/packet/clientPackets/auth/SignUpSuccess.h \
        ../src/network/packet/clientPackets/auth/SignUpFailure.h \
        ../src/network/packet/serverPackets/auth/SignUp.h \
        ../src/network/packet/serverPackets/auth/decorator/SignUpDecorator.h \
        ../src/network/packet/clientPackets/auth/SignInSuccess.h \
        ../src/network/packet/clientPackets/auth/SignInFailure.h \
        ../src/network/packet/serverPackets/auth/decorator/SignInDecorator.h \
        ../src/network/packet/serverPackets/auth/SignIn.h \
        ../src/network/packet/serverPackets/auth/decorator/SignOutDecorator.h \
        ../src/network/packet/clientPackets/auth/SignOutSuccess.h \
        ../src/network/packet/clientPackets/auth/SignOutFailure.h \
        ../src/network/packet/serverPackets/auth/SignOut.h \
        ../src/network/packet/factory/PacketFactory.h \
        ../src/network/packet/factory/ServerPacketFactory.h \
        ../src/network/server/Server.h \
        ../src/network/session/Session.h \
        ../src/utils/definitions.h \
        ../src/utils/utils.h \
        ../src/utils/qt_utils.h \
        ../src/utils/DBLogger.h \
        ../src/utils/ErrorCodes.h \
        ../src/utils/Exception.h \
        ../src/utils/registry/RegistryHelper.h \
    ../src/network/session/server_session/ServerSession.h

SOURCES += \
        ../src/context/CBServerContext.cpp \
        ../src/database/DAO/UsersDAO.cpp \
        ../src/database/mysql/command.cpp \
        ../src/network/packet/clientPackets/ErrorHappend.cpp \
        ../src/network/packet/clientPackets/auth/SignUpSuccess.cpp \
        ../src/network/packet/clientPackets/auth/SignUpFailure.cpp \
        ../src/network/packet/serverPackets/auth/SignUp.cpp \
        ../src/network/packet/serverPackets/auth/decorator/SignUpDecorator.cpp \
        ../src/network/packet/clientPackets/auth/SignInSuccess.cpp \
        ../src/network/packet/clientPackets/auth/SignInFailure.cpp \
        ../src/network/packet/serverPackets/auth/SignIn.cpp \
        ../src/network/packet/serverPackets/auth/decorator/SignInDecorator.cpp \
        ../src/network/packet/serverPackets/auth/decorator/SignOutDecorator.cpp \
        ../src/network/packet/clientPackets/auth/SignOutSuccess.cpp \
        ../src/network/packet/clientPackets/auth/SignOutFailure.cpp \
        ../src/network/packet/serverPackets/auth/SignOut.cpp \
        ../src/network/packet/PacketProcessor.cpp \
        ../src/network/packet/factory/PacketFactory.cpp \
        ../src/network/packet/factory/ServerPacketFactory.cpp \
        ../src/network/server/Server.cpp \
        ../src/network/session/server_session/ServerSession.cpp \
        ../src/network/session/Session.cpp \
        ../src/utils/utils.cpp \
        ../src/utils/registry/RegistryHelper.cpp \
        ../src/utils/qt_utils.cpp \
        ../src/utils/DBLogger.cpp \
    main.cpp \
