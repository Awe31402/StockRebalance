#-------------------------------------------------
#
# Project created by QtCreator 2021-07-21T14:35:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11


TARGET = StockRebalance
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    loadportfoliowidget.cpp \
    curl_utils.cpp \
    quote.cpp \
    spot.cpp \
    time_utils.cpp \
    stockinfo.cpp \
    loadsockinfoworker.cpp \
    stockwaveform.cpp \
    qcustomplot.cpp

HEADERS += \
        mainwindow.h \
    loadportfoliowidget.h \
    curl_utils.hpp \
    quote.hpp \
    spot.hpp \
    time_utils.hpp \
    stockinfo.h \
    loadsockinfoworker.h \
    stockwaveform.h \
    qcustomplot.h

FORMS += \
        mainwindow.ui \
    loadportfoliowidget.ui \
    stockwaveform.ui

LIBS += -lcurl
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
