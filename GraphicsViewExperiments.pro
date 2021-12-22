QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dockwidget.cpp \
    flowchartitems.cpp \
    guideline.cpp \
    itembase.cpp \
    itembutton.cpp \
    itemresizer.cpp \
    main.cpp \
    mainwindow.cpp \
    resizehandle.cpp \
    scene.cpp \
    shapeitemdrag.cpp \
    textitem.cpp \
    uppertoolbar.cpp \
    view.cpp

HEADERS += \
    dockwidget.h \
    flowchartitems.h \
    guideline.h \
    itembase.h \
    itembutton.h \
    itemresizer.h \
    mainwindow.h \
    resizehandle.h \
    scene.h \
    shapeitemdrag.h \
    textitem.h \
    uppertoolbar.h \
    view.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
