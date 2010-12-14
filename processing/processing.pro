TEMPLATE = lib
TARGET = processing

CONFIG += debug

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += Corrections.cc

HEADERS += Corrections.hh

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/setup

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -lsetup

include(../root.pri)