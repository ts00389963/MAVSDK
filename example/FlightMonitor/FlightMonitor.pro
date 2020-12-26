QT       += core gui widgets serialport printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connectdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    connectdialog.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    connectdialog.ui \
    mainwindow.ui

unix:LIBS += -L/usr/local/lib -lmavsdk -lmavsdk_action -lmavsdk_telemetry

INCLUDEPATH = /usr/local/include/mavsdk

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc

DISTFILES +=
