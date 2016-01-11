#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T19:44:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwoLibrary
TEMPLATE = app
CONFIG += C++11
QMAKE_CXXFLAGS += /wd4996

SOURCES += main.cpp\
        index.cpp \
    login.cpp \
    data.cpp \
    configure.cpp \
    usermanagement.cpp \
    adminframe.cpp \
    addaccount.cpp \
    addbook.cpp \
    bookmanagement.cpp \
    userframe.cpp \
    bookinfo.cpp

HEADERS  += index.h \
    login.h \
    data.h \
    configure.h \
    usermanagement.h \
    adminframe.h \
    addaccount.h \
    addbook.h \
    bookmanagement.h \
    userframe.h \
    bookinfo.h

FORMS    += index.ui \
    login.ui \
    adminframe.ui \
    addaccount.ui \
    addbook.ui \
    userframe.ui \
    bookinfo.ui

RESOURCES += \
    images.qrc \

DISTFILES +=
