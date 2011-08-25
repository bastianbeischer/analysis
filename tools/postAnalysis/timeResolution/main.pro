TEMPLATE = app
TARGET = timeResolution

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/global \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lrootplot \
  -lglobal \
  -lpostAnalysis

CLASSES = \
  MainWindow \
  TimeOfFlightHistogram \
  TimeResolutionPlot \
  TimeResolutionAnalysis \
  ResiduePlot \
  PullPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
