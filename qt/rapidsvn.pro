# -------------------------------------------------
# Project created by QtCreator 2010-06-10T21:24:48
# -------------------------------------------------
QT += xml
TARGET = rapidsvn
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp
HEADERS += mainwindow.h \
    config.h \
    listener.hpp \
    utils.hpp \
    ids.hpp
FORMS += mainwindow.ui
RESOURCES += resource.qrc

# TODO search for libraries
INCLUDEPATH += /usr/include/subversion-1 \
    /usr/include/apr-1.0
INCLUDEPATH += .
