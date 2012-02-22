TEMPLATE = app
TARGET = likelihoodAnalysis

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/widgets \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/global \
  $$TOPLEVEL/event \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/likelihood \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lwidgets \
  -lrootplot \
  -lglobal \
  -lsetup \
  -lSimpleEvent \
  -ltracking \
  -lprocessing \
  -lparticle \
  -llikelihood \
  -lpostAnalysis

CLASSES = \
  MainWindow \
  LogLikelihoodPlot \
  LogLikelihoodProjectionPlot \
  EfficiencyPlot \
  RejectionPlot \
  RejectionEfficiencyPlot \
  LikelihoodPDFPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
