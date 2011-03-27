TEMPLATE = app
TARGET = photonTravelTime

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -ltracking \
  -lrootplot \
  -lsetup \
  -lprocessing \
	-lglobal \
  -lpostAnalysis

CLASSES = \
  MainWindow \
  PostAnalysisPlot \
  TimeDifferenceFunction \
  TimeDifferencePlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
