TEMPLATE = app
TARGET = parser

CONFIG += debug

TOPLEVEL = ../..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/bin
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

# Forms
FORMS =

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/setup
DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -lsetup

# Headers
HEADERS = Converter.hh \
          DataManager.hh \
          SingleFile.hh

# Sources
SOURCES = main.cc \
          Converter.cc \
          DataManager.cc \
          SingleFile.cc
                  
include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include(perdaix.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
