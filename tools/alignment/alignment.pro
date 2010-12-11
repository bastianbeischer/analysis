TEMPLATE = app
TARGET = alignment

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

INCLUDEPATH += $$TOPLEVEL/alignment

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lalignment -lSimpleEvent -lsetup -ltracking -lmillepede

# Headers
HEADERS = 

# Sources
SOURCES = main.cc

include($$TOPLEVEL/root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
