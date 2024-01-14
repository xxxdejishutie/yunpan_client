QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += UNICODE

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


LIBS += -l"Ws2_32"
SOURCES += \
    cmytheadpool.cpp \
    dialog.cpp \
    getipport.cpp \
    main.cpp \
    md5.cpp \
    menu.cpp \
    shareview.cpp \
    tcpinet.cpp \
    tcpkernel.cpp \
    widget.cpp

HEADERS += \
    cmytheadpool.h \
    dialog.h \
    getipport.h \
    ikerkel.h \
    inet.h \
    md5.h \
    menu.h \
    packdef.h \
    shareview.h \
    tcpinet.h \
    tcpkernel.h \
    widget.h

FORMS += \
    dialog.ui \
    menu.ui \
    shareview.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
