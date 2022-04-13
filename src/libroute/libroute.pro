QT += core gui widgets
TEMPLATE = lib

CONFIG += c++11 shared

TARGET = route

HEADERS += \
    astar.h \
    grid.h \
    node.h \
    orthogonalconnector.h \
    orthogonalconnectorauto.h \
    orthogonalconnectorfunctions.h \
    orthogonalconnectormanual.h \
    pointgraph.h \
    shapeinfo.h \
    waypoint.h

SOURCES += \
    astar.cpp \
    grid.cpp \
    node.cpp \
    orthogonalconnector.cpp \
    orthogonalconnectorauto.cpp \
    orthogonalconnectorfunctions.cpp \
    orthogonalconnectormanual.cpp \
    pointgraph.cpp \
    shapeinfo.cpp \
    waypoint.cpp
