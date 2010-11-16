TEMPLATE = lib
TARGET = rootPlot

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += RootPlot.cc \
           H1DPlot.cc \
           H2DPlot.cc \
           PerdaixDisplay.cc

HEADERS += RootPlot.hh \
           H1DPlot.hh \
           H2DPlot.hh \
           PerdaixDisplay.hh

INCLUDEPATH +=

DEPENDPATH += $$INCLUDEPATH

LIBS +=

include(../root.pri)
