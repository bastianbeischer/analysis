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
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lprocessing \
  -lparticle \
  -ltracking \
  -lrootplot \
  -lsetup \
  -lglobal

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
  Chi2VsMomentumPlot \
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
  TRDMoPVTimeEvolutionPlot \
  TRDOccupancyPlot \
  TRDSpectrumPlot \
  TimeResolutionPlot \
  TotalEnergyDepositionPlot \
  TotalEnergyDepositionTRDvsTrackerPlot \
  TOFEfficiencyPlot \
  TrackingEfficiencyVsMomentumPlot \
  TOTMomentumCorrelation \
  TOTBetaCorrelation \
  TOTPlot \
  TOTLayerPlot \
  TOTIonizationCorrelation \
  TOTTemperatureCorrelationPlot \
  TOTTimeCorrelationPlot \
  TOFAlignment \
  TemperatureTimePlot \
  ChannelTriggerProbabilityPlot \
  TOFTimeShiftTriggerPlot \
  HeightTimePlot \
  TriggerRateTimePlot \
  SensorTimePlot \
  MCTotalEnergyDepositionTRDvsTrackerPlot \
  MCTRDSpectrumPlot

SOURCES += \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)

RESOURCES += \
    resources.qrc
