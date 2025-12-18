# qt_gui.pro - Qt project configuration

QT += core widgets gui

CONFIG += c++17

TARGET = coreMusicPlayerGUI
TEMPLATE = app

SOURCES += \
    src/gui/main_window.cpp

HEADERS += \
    src/gui/main_window.h

FORMS += \
    src/gui/main_window.ui

INCLUDEPATH += src