TEMPLATE = app
TARGET = sellerparser

QT = core  \
     gui   \
     sql

include(common.pro)

HEADERS += \
           sellerparser/abstractparser.h    \
           sellerparser/abstractsaver.h     \
           sellerparser/dummysaver.h        \
           sellerparser/moscowcsvparser.h   \
           sellerparser/sellergoods.h       \
           sellerparser/sellerimporter.h    \
           sellerparser/sqlsaver.h          \
           ui/sellerwidget/sellerwidget.h

SOURCES += \
           sellerparser/abstractparser.cpp  \
           sellerparser/abstractsaver.cpp   \
           sellerparser/dummysaver.cpp      \
           sellerparser/moscowcsvparser.cpp \
           sellerparser/sellergoods.cpp     \
           sellerparser/sellerimporter.cpp  \
           sellerparser/sqlsaver.cpp        \
           ui/sellerwidget/sellerwidget.cpp \
           app/sellerparser/main.cpp


FORMS +=   ui/sellerwidget/sellerwidget.ui
