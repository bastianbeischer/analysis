TEMPLATE = app
TARGET = likelihoodAnalysis

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
  LogLikelihoodPlot \
  LogLikelihoodProjectionPlot \
  EfficiencyPlot \
  RejectionPlot \
  RejectionEfficiencyPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
