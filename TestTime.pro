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
    filterutilitydialog.cpp \
    inserttimedialog.cpp \
    #../../../../OneDrive/Documents/QT Test Folder/General_Library/mattcalculations.cpp \
    #../../../../OneDrive/Documents/QT Test Folder/General_Library/mattcalculations.cpp

HEADERS  += mainwindow.h \
    calculations.h \
    loadfiledialog.h \
    constantants.h \
    changelogtitledialog.h \
    filterutilitydialog.h \
    inserttimedialog.h \
    #../../../../OneDrive/Documents/QT Test Folder/General_Library/mattcalculations.h \
    #../../../../OneDrive/Documents/QT Test Folder/General_Library/mattcalculations.h

FORMS    += mainwindow.ui \
    loadfiledialog.ui \
    changelogtitledialog.ui \
    filterutilitydialog.ui \
    inserttimedialog.ui

RESOURCES += \
    images.qrc

RC_FILE = res.rc

DISTFILES += \
    res.rc \
    install.nsi

#QMAKE_POST_LINK += windeployqt \"$$system_path($${OUT_PWD}/release/TestTime.exe)\" --dir \"$$system_path($${_PRO_FILE_PWD_}/Deploy)\"
#QMAKE_POST_LINK += && $$QMAKE_COPY \"$$system_path($${OUT_PWD}/release/TestTime.exe)\" \"$$system_path($${_PRO_FILE_PWD_}/Deploy/)\"
