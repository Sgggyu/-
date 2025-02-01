QT       += core gui sql charts concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adddatadialog.cpp \
    chartwindow.cpp \
    filrerdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    sensor1.cpp \
    set.cpp

HEADERS += \
    adddatadialog.h \
    chartwindow.h \
    clickableheader.h \
    filterdialog.h \
    mainwindow.h \
    menu.h \
    sensor1.h \
    set.h

FORMS += \
    adddatadialog.ui \
    chartwindow.ui \
    mainwindow.ui \
    menu.ui \
    sensor1.ui \
    set.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
