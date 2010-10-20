TEMPLATE = app
TARGET = preAnalysis

CONFIG += debug

TOPLEVEL = ../..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/bin
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

INCLUDEPATH += $$TOPLEVEL/event
DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent

# Headers
HEADERS = 

# Sources
SOURCES = main.cc
                  
include($$TOPLEVEL/root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
