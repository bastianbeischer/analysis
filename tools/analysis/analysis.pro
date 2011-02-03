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
               $$TOPLEVEL/global \
               $$TOPLEVEL/processing \
               $$TOPLEVEL/rootplot \
               $$TOPLEVEL/setup \
               $$TOPLEVEL/tracking

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -lprocessing \
                         -ltracking \
                         -lrootplot \
                         -lsetup

# Headers
HEADERS = MainWindow.hh \
          Plotter.hh \
          EventQueue.hh \
          AnalysisThread.hh \
          AnalysisPlot.hh \
          BendingPositionPlot.hh \
          GeometricOccupancyPlot.hh \
          ResidualPlot.hh \
          BendingAnglePlot.hh \
          BendingAnglePositionPlot.hh \
          Chi2Plot.hh \
          Chi2PerNdfPlot.hh \
          TOFPositionCorrelationPlot.hh \
          AlbedosVsMomentumPlot.hh \
          MomentumSpectrumPlot.hh \
          TimeOfFlightPlot.hh \
          GeometricOccupancyProjectionPlot.hh \
          SignalHeightPlot.hh \
          ClusterLengthPlot.hh \
          TimeDifferencePlot.hh \
          TimeOfFlightMomentumCorrelationPlot.hh \
          TRDCalculations.hh \
          TRDClustersOnTrackPlot.hh \
          TRDDistanceInTube.hh \
          TRDDistanceWireToTrackPlot.hh \
          TRDEnergyDepositionOverMomentumPlot.hh \
          TRDSpectrumPlot.hh \
          TRDFitPlot.hh \
          TRDOccupancyPlot.hh \
          TRDEfficiencyPlot.hh \
          TRDMoPVTimeEvolutionPlot.hh \
          TotalEnergyDepositionPlot.hh \
          TimeResolutionPlot.hh

# Sources
SOURCES = main.cc \
          MainWindow.cc \
          Plotter.cc \
          EventQueue.cc \
          AnalysisThread.cc \
          AnalysisPlot.cc \
          BendingPositionPlot.cc \
          GeometricOccupancyPlot.cc \
          ResidualPlot.cc \
          BendingAnglePlot.cc \
          BendingAnglePositionPlot.cc \
          Chi2Plot.cc \
          Chi2PerNdfPlot.cc \
          TOFPositionCorrelationPlot.cc \
          AlbedosVsMomentumPlot.cc \
          MomentumSpectrumPlot.cc \
          TimeOfFlightPlot.cc \
          GeometricOccupancyProjectionPlot.cc \
          SignalHeightPlot.cc \
          ClusterLengthPlot.cc \
          TimeDifferencePlot.cc \
          TimeOfFlightMomentumCorrelationPlot.cc \
          TRDCalculations.cc \
          TRDClustersOnTrackPlot.cc \
          TRDDistanceInTube.cc \
          TRDDistanceWireToTrackPlot.cc \
          TRDEnergyDepositionOverMomentumPlot.cc \
          TRDSpectrumPlot.cc \
          TRDFitPlot.cc \
          TRDOccupancyPlot.cc \
          TRDEfficiencyPlot.cc \
          TRDMoPVTimeEvolutionPlot.cc \
          TotalEnergyDepositionPlot.cc \
          TimeResolutionPlot.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
