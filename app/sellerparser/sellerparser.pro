TEMPLATE = app
TARGET = sellerparser

QT = core gui

include($$(GARSALE)/common.pro)

SOURCES = main.cpp

LIBS += -L/usr/lib/i386-linux-gnu \
        -lsellerparser  \
        -lsellerwidget
