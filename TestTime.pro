#-------------------------------------------------
#
# Project created by QtCreator 2016-01-26T20:52:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestTime
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calculations.cpp \
    loadfiledialog.cpp \
    changelogtitledialog.cpp \
    filterutilitydialog.cpp

HEADERS  += mainwindow.h \
    calculations.h \
    loadfiledialog.h \
    constantants.h \
    changelogtitledialog.h \
    filterutilitydialog.h

FORMS    += mainwindow.ui \
    loadfiledialog.ui \
    changelogtitledialog.ui \
    filterutilitydialog.ui
