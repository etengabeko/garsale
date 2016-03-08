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
           sellerparser/fdbsaver.h          \
           sellerparser/moscowcsvparser.h   \
           sellerparser/sellergoods.h       \
           sellerparser/sellerimporter.h    \
           settings/dbsettings.h            \
           settings/fdbquerysettings.h      \
           settings/settings.h              \
           ui/sellerwidget/sellerwidget.h

SOURCES += \
           sellerparser/abstractparser.cpp  \
           sellerparser/abstractsaver.cpp   \
           sellerparser/dummysaver.cpp      \
           sellerparser/fdbsaver.cpp        \
           sellerparser/moscowcsvparser.cpp \
           sellerparser/sellergoods.cpp     \
           sellerparser/sellerimporter.cpp  \
           settings/dbsettings.cpp          \
           settings/fdbquerysettings.cpp    \
           settings/settings.cpp            \
           ui/sellerwidget/sellerwidget.cpp \
           app/sellerparser/main.cpp


FORMS +=   ui/sellerwidget/sellerwidget.ui

CONF_FILES = etc/garsale.conf
conf.files = $$CONF_FILES
conf.path  = $$(HOME)/.garsale
!win32 {
  conf.extra = chmod a+r $$CONF_FILES
}
INSTALLS += conf
