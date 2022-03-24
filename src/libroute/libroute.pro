QT += core gui widgets
TEMPLATE = lib

CONFIG += c++11 shared

TARGET = route

HEADERS += \
    astar.h \
    grid.h \
    node.h \
    orthogonalconnector.h \
    pointgraph.h \
    shapeinfo.h

SOURCES += \
    astar.cpp \
    grid.cpp \
    orthogonalconnector.cpp \
    pointgraph.cpp
