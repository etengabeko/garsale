TEMPLATE = lib
TARGET = sellerparser

QT = core

include($$(GARSALE)/common.pro)

HEADERS += abstractparser.h    \
           abstractsaver.h     \
           dummysaver.h        \
           moscowcsvparser.h   \
           sellergoods.h       \
           sellerimporter.h

SOURCES += abstractparser.cpp  \
           abstractsaver.cpp   \
           dummysaver.cpp      \
           moscowcsvparser.cpp \
           sellergoods.cpp     \
           sellerimporter.cpp
