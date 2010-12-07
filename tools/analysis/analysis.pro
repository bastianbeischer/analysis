TEMPLATE = app
TARGET = analysis

#CONFIG += debug

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
               $$TOPLEVEL/rootplot \
               $$TOPLEVEL/setup \
               $$TOPLEVEL/tracking

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -ltracking \
                         -lrootplot \
                         -lsetup

# Headers
HEADERS = MainWindow.hh \
          Plotter.hh \
          EventQueue.hh \
          AnalysisThread.hh \
          AnalysisPlot.hh \
          RootPlot.hh \
          H1DPlot.hh \
          H2DPlot.hh \
          BendingPositionPlot.hh \
          GeometricOccupancyPlot.hh \
          ResidualPlot.hh \
          BendingAnglePlot.hh \
          BendingAnglePositionPlot.hh \
          Chi2Plot.hh \
          TOFPositionCorrelationPlot.hh \
          MomentumSpectrumPlot.hh \
          TimeOfFlightPlot.hh \
          GeometricOccupancyProjectionPlot.hh \
          SignalHeightPlot.hh \
          ClusterLengthPlot.hh \
          TimeDifferencePlot.hh

# Sources
SOURCES = main.cc \
          MainWindow.cc \
          Plotter.cc \
          EventQueue.cc \
          AnalysisThread.cc \
          AnalysisPlot.cc \
          RootPlot.cc \
          H1DPlot.cc \
          H2DPlot.cc \
          BendingPositionPlot.cc \
          GeometricOccupancyPlot.cc \
          ResidualPlot.cc \
          BendingAnglePlot.cc \
          BendingAnglePositionPlot.cc \
          Chi2Plot.cc \
          TOFPositionCorrelationPlot.cc \
          MomentumSpectrumPlot.cc \
          TimeOfFlightPlot.cc \
          GeometricOccupancyProjectionPlot.cc \
          SignalHeightPlot.cc \
          ClusterLengthPlot.cc \
          TimeDifferencePlot.cc

include($$TOPLEVEL/root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
