QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = AbduCharts

win32:RC_ICONS = ":/images/app_icon.ico"

INCLUDEPATH += ../
LIBS += -L../libroute -lroute

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editor/connectorhandle.cpp \
    editor/connectoritem.cpp \
    editor/connectoritemmanager.cpp \
    editor/connectorpoint.cpp \
    editor/grid.cpp \
    editor/guidelines.cpp \
    editor/itemresizer.cpp \
    editor/items/decisionshapeitem.cpp \
    editor/items/flowchartshapeitem.cpp \
    editor/items/flowchartshapestextitem.cpp \
    editor/items/flowcharttextitem.cpp \
    editor/items/inoutshapeitem.cpp \
    editor/items/processshapeitem.cpp \
    editor/items/shapeitembase.cpp \
    editor/items/shapeitemcreator.cpp \
    editor/items/terminalshapeitem.cpp \
    editor/items/textitem.cpp \
    editor/resizehandle.cpp \
    editor/scene.cpp \
    editor/undo_commands/addcommand.cpp \
    editor/undo_commands/addconnectorcommand.cpp \
    editor/undo_commands/addshapecommand.cpp \
    editor/undo_commands/addtextcommand.cpp \
    editor/undo_commands/deletecommand.cpp \
    editor/undo_commands/deleteconnectorcommand.cpp \
    editor/undo_commands/deleteshapecommand.cpp \
    editor/undo_commands/deletetextcommand.cpp \
    editor/undo_commands/movecommand.cpp \
    editor/undo_commands/moveshapecommand.cpp \
    editor/undo_commands/movetextcommand.cpp \
    editor/undo_commands/resizecommand.cpp \
    editor/view.cpp \
    main.cpp \
    ui/bottomtoolbar.cpp \
    ui/colorpickerbutton.cpp \
    ui/itembutton.cpp \
    ui/itemlibrarydockkwidget.cpp \
    ui/mainwindow.cpp \
    ui/shapeitemdrag.cpp \
    ui/uppertoolbar.cpp \
    ui/zoomcombobox.cpp \
    ui/zoomlineedit.cpp \
    ui/zoompicker.cpp \
    ui/zoomvalidator.cpp

HEADERS += \
    editor/connectorhandle.h \
    editor/connectoritem.h \
    editor/connectoritemmanager.h \
    editor/connectorpoint.h \
    editor/grid.h \
    editor/guidelines.h \
    editor/itemresizer.h \
    editor/items/decisionshapeitem.h \
    editor/items/flowchartshapeitem.h \
    editor/items/flowchartshapeitems.h \
    editor/items/flowchartshapestextitem.h \
    editor/items/flowcharttextitem.h \
    editor/items/flowcharttextitems.h \
    editor/items/inoutshapeitem.h \
    editor/items/processshapeitem.h \
    editor/items/shapeitembase.h \
    editor/items/shapeitemcreator.h \
    editor/items/terminalshapeitem.h \
    editor/items/textitem.h \
    editor/resizehandle.h \
    editor/scene.h \
    editor/undo_commands/addcommand.h \
    editor/undo_commands/addconnectorcommand.h \
    editor/undo_commands/addshapecommand.h \
    editor/undo_commands/addtextcommand.h \
    editor/undo_commands/deletecommand.h \
    editor/undo_commands/deleteconnectorcommand.h \
    editor/undo_commands/deleteshapecommand.h \
    editor/undo_commands/deletetextcommand.h \
    editor/undo_commands/movecommand.h \
    editor/undo_commands/moveshapecommand.h \
    editor/undo_commands/movetextcommand.h \
    editor/undo_commands/resizecommand.h \
    editor/undo_commands/undocommands.h \
    editor/view.h \
    ui/bottomtoolbar.h \
    ui/colorpickerbutton.h \
    ui/itembutton.h \
    ui/itemlibrarydockkwidget.h \
    ui/mainwindow.h \
    ui/shapeitemdrag.h \
    ui/uppertoolbar.h \
    ui/zoomcombobox.h \
    ui/zoomlineedit.h \
    ui/zoompicker.h \
    ui/zoomvalidator.h

RESOURCES += \
    ../../resources/images.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
