TEMPLATE = app
TARGET = residuals

#CONFIG += debug

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
               $$TOPLEVEL/tracking \
               $$TOPLEVEL/setup

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -ltracking \
                         -lsetup

# Headers
HEADERS = Plotter.hh \
          PlotHits.hh \
          ResidualPlot.hh

# Sources
SOURCES = main.cc \
          Plotter.cc \
          PlotHits.cc \
          ResidualPlot.cc

include($$TOPLEVEL/root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
