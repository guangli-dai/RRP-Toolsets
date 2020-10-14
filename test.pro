#-------------------------------------------------
#
# Project created by QtCreator 2019-06-10T11:32:53
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
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
        asc.cpp \
        main.cpp \
        mainwindow.cpp \
        partition.cpp \
        partition_dialog.cpp \
        pcg.cpp \
        st.cpp \
        task.cpp \
        task_dialog.cpp

HEADERS += \
        asc.h \
        mainwindow.h \
        partition.h \
        partition_dialog.h \
        pcg.h \
        st.h \
        task.h \
        task_dialog.h

FORMS += \
        asc.ui \
        mainwindow.ui \
        partition_dialog.ui \
        pcg.ui \
        st.ui \
        task_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
