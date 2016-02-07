TEMPLATE = app
TARGET = sellerparser

OBJECTS_DIR = ./.o
MOC_DIR = ./.moc
UI_DIR  = ./.ui
RCC_DIR = ./.rcc
INCLUDEPATH += ./.ui

CONFIG += debug    \
          warn_on

QT += core

QMAKE_CXXFLAGS += -std=c++11   \
                  -Werror      \
                  -Wextra      \
                  -Winit-self  \
                  -Wundef      \
                  -Wcast-qual  \

QMAKE_CFLAGS +=   -Werror

HEADERS += abstractparser.h    \
           abstractsaver.h     \
           dummysaver.h        \
           moscowcsvparser.h   \
           sellergoods.h       \
           sellerimporter.h    \

SOURCES += main.cpp            \
           abstractparser.cpp  \
           abstractsaver.cpp   \
           dummysaver.cpp      \
           moscowcsvparser.cpp \
           sellergoods.cpp     \
           sellerimporter.cpp  \
