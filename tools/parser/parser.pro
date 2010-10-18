TEMPLATE = app
TARGET = parser

CONFIG += debug

# Seperate source & build dirs
DESTDIR     = ./
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

# Forms
FORMS =

TOPLEVEL = ../..
INCLUDEPATH += ../../event
DEPENDPATH += $$INCLUDEPATH

LIBS += -L../../event -levent

# Headers
HEADERS = Converter.hh \
          DataManager.hh \
          SingleFile.hh

# Sources
SOURCES = main.cc \
          Converter.cc \
          DataManager.cc \
          SingleFile.cc
                  
include(perdaix.pri)
include(root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
