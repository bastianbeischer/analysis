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
  AlbedosVsMomentumPlot \
  AnalysisPlot \
  BendingAnglePlot \
  BendingAnglePositionPlot \
  BendingPositionPlot \
  BetaMomentumCorrelationPlot \
  BetaPlot \
  Chi2PerNdfPlot \
  Chi2Plot \
  ClusterLengthPlot \
  ClusterShapePlot \
  GeometricOccupancyPlot \
  GeometricOccupancyProjectionPlot \
  MainWindow \
  MomentumSpectrumPlot \
  Plotter \
  ResidualPlot \
  SignalHeightPlot \
  SingleLayerTrackingEfficiencyPlot \
  MultiLayerTrackingEfficiencyPlot \
  CutStatisticsPlot \
  TOFPositionCorrelationPlot \
  TOFTimeDifferencePlot \
  TOFTimeShiftPlot \
  TotalSignalHeightPlot \
  TrackerLayerStatisticsPlot \
  TRDCalculations \
  TRDClustersOnTrackPlot \
  TRDDistanceInTube \
  TRDDistanceWireToTrackPlot \
  TRDEfficiencyPlot \
  TRDEnergyDepositionOverMomentumPlot \
  TRDFitPlot \
  TRDMoPVTimeEvolutionPlot \
  TRDOccupancyPlot \
  TRDSpectrumPlot \
  TimeResolutionPlot \
  TotalEnergyDepositionPlot \
  TotalEnergyDepositionTRDvsTrackerPlot \
  TOFEfficiencyPlot \
  TrackingEfficiencyVsMomentumPlot \
  TOTMomentumCorrelation \
  TOTPlot \
  TOTPerBarPlot \
  TOTPerLayerPlot \
  TOTDetectorIonisationCorrelation \
  TOTTemperatureCorrelationPlot \
  TOTTimeCorrelationPlot \
  TOFAlignment

SOURCES += \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)

RESOURCES += \
    resources.qrc
