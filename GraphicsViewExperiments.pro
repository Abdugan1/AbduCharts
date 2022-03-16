QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

win32:RC_ICONS = ":/images/app_icon.ico"

INCLUDEPATH += $$PWD/src

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/editor/grid.cpp \
    src/editor/guidelines.cpp \
    src/editor/itemresizer.cpp \
    src/editor/items/decisionshapeitem.cpp \
    src/editor/items/flowchartshapeitem.cpp \
    src/editor/items/inoutshapeitem.cpp \
    src/editor/items/processshapeitem.cpp \
    src/editor/items/shapeitembase.cpp \
    src/editor/items/shapeitemcreator.cpp \
    src/editor/items/terminalshapeitem.cpp \
    src/editor/resizehandle.cpp \
    src/editor/scene.cpp \
    src/editor/textitems.cpp \
    src/editor/view.cpp \
    src/main.cpp \
    src/ui/bottomtoolbar.cpp \
    src/ui/colorpickerbutton.cpp \
    src/ui/itembutton.cpp \
    src/ui/itemlibrarydockkwidget.cpp \
    src/ui/mainwindow.cpp \
    src/ui/shapeitemdrag.cpp \
    src/ui/undocommands.cpp \
    src/ui/uppertoolbar.cpp \
    src/ui/zoomcombobox.cpp \
    src/ui/zoomlineedit.cpp \
    src/ui/zoompicker.cpp \
    src/ui/zoomvalidator.cpp

HEADERS += \
    src/editor/grid.h \
    src/editor/guidelines.h \
    src/editor/itemresizer.h \
    src/editor/items/decisionshapeitem.h \
    src/editor/items/flowchartshapeitem.h \
    src/editor/items/flowchartshapeitems.h \
    src/editor/items/inoutshapeitem.h \
    src/editor/items/processshapeitem.h \
    src/editor/items/shapeitembase.h \
    src/editor/items/shapeitemcreator.h \
    src/editor/items/terminalshapeitem.h \
    src/editor/resizehandle.h \
    src/editor/scene.h \
    src/editor/textitems.h \
    src/editor/view.h \
    src/ui/bottomtoolbar.h \
    src/ui/colorpickerbutton.h \
    src/ui/itembutton.h \
    src/ui/itemlibrarydockkwidget.h \
    src/ui/mainwindow.h \
    src/ui/shapeitemdrag.h \
    src/ui/undocommands.h \
    src/ui/uppertoolbar.h \
    src/ui/zoomcombobox.h \
    src/ui/zoomlineedit.h \
    src/ui/zoompicker.h \
    src/ui/zoomvalidator.h

RESOURCES += \
    resources/images.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
