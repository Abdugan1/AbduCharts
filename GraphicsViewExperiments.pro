QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    guideline.cpp \
    itembase.cpp \
    itemresizer.cpp \
    main.cpp \
    myitem.cpp \
    resizehandle.cpp \
    scene.cpp \
    textitem.cpp \
    view.cpp

HEADERS += \
    guideline.h \
    itembase.h \
    itemresizer.h \
    myitem.h \
    resizehandle.h \
    scene.h \
    textitem.h \
    view.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
