#-------------------------------------------------
#
# Project created by QtCreator 2018-03-30T12:05:02
#
#-------------------------------------------------

QT       += core widgets network

# TARGET = client

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        client.cpp \
    frameconsumer.cpp \
    hrconsumer.cpp \
    fpsconsumer.cpp \
    infoconsumer.cpp

HEADERS += \
        client.h \
    consumer.h \
    frameconsumer.h \
    hrconsumer.h \
    fpsconsumer.h \
    infoconsumer.h

DISTFILES +=



target.path = /home/pi
#LIBS += -L /home/raphy/mazupi2/sysroot/usr/lib/libpigpiod_if2.so
#LIBS += -lpigpio
INSTALLS += target
