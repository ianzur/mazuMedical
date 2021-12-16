QT += gui
QT += network
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    server.cpp \
    frameprovider.cpp \
    processorthread.cpp \
    pyramids.cpp \
    hrprovider.cpp \
    fpsprovider.cpp \
    infoprovider.cpp


HEADERS += \
    server.h \
    provider.h \
    frameprovider.h \
    processorthread.h \
    pyramids.h \
    hrprovider.h \
    fpsprovider.h \
    infoprovider.h


LIBS += -lopencv_core -lopencv_videoio -lopencv_objdetect -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

INCLUDEPATH += /usr/include/opencv4/
DEPENDPATH +=  /usr/include/opencv4/

DISTFILES += \
    haarcascade_frontalface_alt2.xml
