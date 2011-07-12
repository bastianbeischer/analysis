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
  -lsetup

CLASSES = \
  AlbedosVsMomentumPlot \
  AnalysisPlot \
  BendingAnglePlot \
  BendingAnglePositionPlot \
  BendingPositionPlot \
  BetaMomentumCorrelationPlot \
  BetaPlot \
  BeamProfilePlot \
  Chi2PerNdfPlot \
  Chi2Plot \
  Chi2VsMomentumPlot \
  PlotCollection \
  TRDSpectrumPlotCollection \
  TRDSpectrumCherenkovPlotCollection \
  TRDSpectrumVsTimePlotCollection \
  TRDSpectrumVsTemperaturePlotCollection \
  TRDSpectrumVsPressurePlotCollection \
  ClusterLengthPlot \
  ClusterShapePlot \
  GeometricOccupancyPlot \
  MainWindow \
  MomentumSpectrumPlot \
  Plotter \
  ResidualPlot \
  ResidualPlotMC \
  SignalHeightPlot \
  SignalHeight2DPlot \
  SingleLayerTrackingEfficiencyPlot \
  MultiLayerTrackingEfficiencyPlot \
  CutStatisticsPlot \
  TOFPositionCorrelationPlot \
  TOFTimeDifferencePlot \
  TOFTimeDifferencePlotCollection \
  TOFTimeShiftPlot \
  TOFTimeShiftPlotCollection \
  TotalSignalHeightPlot \
  TrackerLayerStatisticsPlot \
  TRDCalculations \
  TRDClustersOnTrackPlot \
  TRDDistanceInTube \
  TRDDistanceWireToTrackPlot \
  TRDEfficiencyPlot \
  TRDEnergyDepositionOverMomentumPlot \
  TRDTimeCorrectionPlot \
  TRDOccupancyPlot \
  TRDSpectrumPlot \
  TRDSpectrumCherenkovPlot \
  TRDSpectrumVsTimePlot \
  TRDSpectrumVsTemperaturePlot \
  TRDSpectrumVsPressurePlot \
  TimeResolutionPlot \
  TimeResolutionPlotCollection \
  TotalEnergyDepositionPlot \
  TotalEnergyDepositionTRDvsTrackerPlot \
  TOFEfficiencyPlot \
  TrackingEfficiencyVsMomentumPlot \
  TOTMomentumCorrelation \
  TOTBetaCorrelation \
  TOTLayer \
  TOTPlot \
  TOTLayerPlot \
  TOTLayerCollection \
  TOTIonizationCorrelation \
  TOTTemperatureCorrelationPlotCollection \
  TOTTimeCorrelationPlotCollection \
  TOTTemperatureCorrelationPlot \
  TOTTimeCorrelationPlot \
  TemperatureTimePlot \
  PressureTimePlot \
  ChannelTriggerProbabilityPlot \
  TOFTimeShiftTriggerPlot \
  HeightTimePlot \
  TriggerRateTimePlot \
  SensorTimePlot \
  SettingTimePlot \
  MCTotalEnergyDepositionTRDvsTrackerPlot \
  MCTRDSpectrumPlot \
  MCTRDCalibrationPlot \
  RigidityResolutionPlot \
  MCRigidityResolutionPlot \
  TestbeamRigidityResolutionPlot \
  PMTPlot \
  PMTCorrelationPlot \
  ZSquareTRDPlot \
  TOFBarShiftPlot \
  TOFBarShiftPlotCollection \
  TimeReconstructionPlot

SOURCES += \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)

RESOURCES += \
    resources.qrc
