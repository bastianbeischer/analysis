TEMPLATE = app
TARGET = eventdisplay

CONFIG += debug

TOPLEVEL = ../..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/bin
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

# Forms
FORMS+= MainWindow.ui

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/processing \
               $$TOPLEVEL/rootplot \
               $$TOPLEVEL/tracking \
               $$TOPLEVEL/setup

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -lprocessing \
                         -lrootplot \
                         -ltracking \
                         -lsetup

# Headers
HEADERS = Plotter.hh \
          HitsPlot.hh \
          MainWindow.hh

# Sources
SOURCES = main.cc \
          Plotter.cc \
          HitsPlot.cc \
          MainWindow.cc

include($$TOPLEVEL/root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
