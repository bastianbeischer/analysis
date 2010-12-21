TEMPLATE = lib
TARGET = rootplot

CONFIG += debug

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
           GraphPlot.cc \
           PerdaixDisplay.cc

HEADERS += RootPlot.hh \
           H1DPlot.hh \
           H2DPlot.hh \
           GraphPlot.hh \
           PerdaixDisplay.hh

INCLUDEPATH +=

DEPENDPATH += $$INCLUDEPATH

LIBS +=

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
