TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lpthread

SOURCES += \
    Thread.cpp \
    Socket.cpp \
    Parser.cpp \
    Handler.cpp \
    EventLoopThreadPool.cpp \
    EventLoopThread.cpp \
    EventLoop.cpp \
    Epoll.cpp \
    Buffer.cpp \
    Server.cpp

HEADERS += \
    Mutex.h \
    Thread.h \
    Socket.h \
    Parser.h \
    Handler.h \
    EventLoopThreadPool.h \
    EventLoopThread.h \
    EventLoop.h \
    Epoll.h \
    CurrentThread.h \
    Condition.h \
    Buffer.h \
    Base.h \
    Setting.h

DISTFILES += \
    README.md

