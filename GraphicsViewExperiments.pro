QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colorpickerbutton.cpp \
    flowchartitems.cpp \
    grid.cpp \
    guidelines.cpp \
    itembutton.cpp \
    itemlibrarydockkwidget.cpp \
    itemresizer.cpp \
    main.cpp \
    mainwindow.cpp \
    resizehandle.cpp \
    scene.cpp \
    shapeitembase.cpp \
    shapeitemdrag.cpp \
    textitems.cpp \
    undocommands.cpp \
    uppertoolbar.cpp \
    view.cpp

HEADERS += \
    colorpickerbutton.h \
    flowchartitems.h \
    grid.h \
    guidelines.h \
    itembutton.h \
    itemlibrarydockkwidget.h \
    itemresizer.h \
    mainwindow.h \
    resizehandle.h \
    scene.h \
    shapeitembase.h \
    shapeitemdrag.h \
    textitems.h \
    undocommands.h \
    uppertoolbar.h \
    view.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
