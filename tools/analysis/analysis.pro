TEMPLATE = app
TARGET = analysis

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
               $$TOPLEVEL/tracking \
               $$TOPLEVEL/setup

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -ltracking \
                         -lsetup

# Headers
HEADERS = MainWindow.hh \
          Plotter.hh \
          AnalysisPlot.hh \
          H1DPlot.hh \
          H2DPlot.hh \
          BendingPositionPlot.hh \
          GeometricOccupancyPlot.hh \
          ResidualPlot.hh \
          BendingAnglePlot.hh

# Sources
SOURCES = main.cc \
          MainWindow.cc \
          Plotter.cc \
          AnalysisPlot.cc \
          H1DPlot.cc \
          H2DPlot.cc \
          BendingPositionPlot.cc \
          GeometricOccupancyPlot.cc \
          ResidualPlot.cc \
          BendingAnglePlot.cc

include($$TOPLEVEL/root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
