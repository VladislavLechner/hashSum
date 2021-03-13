QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

BOOST_INCLUDES = D:\boost\boost_1_74_0\

SOURCES += \
        main.cpp

# LIBS += "D:\boost\boost_1_74_0\stage\lib\libboost_filesystem-mgw5-mt-d-x32-1_74.dll"
LIBS += /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
DESTDIR = output

#INCLUDEPATH += \ $$ BOOST_INCLUDES \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
