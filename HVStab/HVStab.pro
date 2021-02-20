#-------------------------------------------------
#
# Project created by QtCreator 2019-09-20T16:06:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = HVStab
TEMPLATE = app
LIBS +=-lpthread -lutil -lusb-1.0
QMAKE_CFLAGS +=-DHAVE_USB -DHAVE_LIBUSB10  -fPIC
QMAKE_CXXFLAGS +=-DHAVE_USB -DHAVE_LIBUSB10  -fPIC
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    drs/DRS.cpp \
    drs/averager.cpp \
    drs/drs_exam_multi.cpp \
    drs/drsampmesure.cpp \
    drs/musbstd.c \
    drs/mxml.c \
    drs/mylib.cpp \
    drs/polyfit.c \
    drs/strlcpy.c \
    drsbox.cpp \
    mainwindow.cpp \
    crate.cpp \
    module.cpp \
    device.cpp \
    hv.cpp \
    loadsetupdata.cpp \
    pugixml.cc \
    snmpwraper.cpp \
    snmpcomands.cpp \
    qcustomplot.cpp \
    mytimer.cpp

HEADERS  += mainwindow.h \
    crate.h \
    drs/DRS.h \
    drs/averager.h \
    drs/drsampmesure.h \
    drs/musbstd.h \
    drs/mxml.h \
    drs/polyfit.h \
    drs/strlcpy.h \
    drsbox.h \
    module.h \
    device.h \
    hv.h \
    loadsetupdata.h \
    pugixml.hh \
    pugiconfig.hh \
    snmpwraper.h \
    snmpcomands.h \
    qcustomplot.h \
    mytimer.h

FORMS    += mainwindow.ui

DISTFILES += \
    config.xml \
    configMTAS.xml
