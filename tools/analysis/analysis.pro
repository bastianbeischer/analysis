TEMPLATE = app
TARGET = analysis

CONFIG += debug

TOPLEVEL = ../..

FORMS += \
  MainWindow.ui

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lprocessing \
  -ltracking \
  -lrootplot \
  -lsetup

CLASSES = \
  MainWindow \
  Plotter \
  EventQueue \
  AnalysisThread \
  AnalysisPlot \
  BendingPositionPlot \
  GeometricOccupancyPlot \
  ResidualPlot \
  BendingAnglePlot \
  BendingAnglePositionPlot \
  Chi2Plot \
  Chi2PerNdfPlot \
  TOFPositionCorrelationPlot \
  AlbedosVsMomentumPlot \
  MomentumSpectrumPlot \
  BetaPlot \
  GeometricOccupancyProjectionPlot \
  SignalHeightPlot \
  ClusterLengthPlot \
  TimeDifferencePlot \
  TimeOfFlightMomentumCorrelationPlot \
  TRDCalculations \
  TRDClustersOnTrackPlot \
  TRDDistanceInTube \
  TRDDistanceWireToTrackPlot \
  TRDEnergyDepositionOverMomentumPlot \
  TRDSpectrumPlot \
  TRDFitPlot \
  TRDOccupancyPlot \
  TRDEfficiencyPlot \
  TRDMoPVTimeEvolutionPlot \
  TotalEnergyDepositionPlot \
  TimeResolutionPlot

SOURCES += \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
