#-------------------------------------------------
#
# Project created by QtCreator 2014-03-11T00:18:38
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = suspend-server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    networklistener.cpp

HEADERS += \
    networklistener.h

unix:!macx {
    QT += dbus
}
