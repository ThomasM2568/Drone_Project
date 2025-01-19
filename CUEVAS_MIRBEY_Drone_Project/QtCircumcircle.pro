QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvas.cpp \
    determinant.cpp \
    drone.cpp \
    graph.cpp \
    main.cpp \
    mainwindow.cpp \
    mypolygon.cpp \
    node.cpp \
    server.cpp \
    triangle.cpp \
    vector2d.cpp \
    voronoi.cpp

HEADERS += \
    canvas.h \
    determinant.h \
    drone.h \
    graph.h \
    mainwindow.h \
    mypolygon.h \
    node.h \
    server.h \
    triangle.h \
    vector2d.h \
    voronoi.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    models/mesh1.json \
    models/mesh2.json

INCLUDEPATH += $$PWD/canvas.h
