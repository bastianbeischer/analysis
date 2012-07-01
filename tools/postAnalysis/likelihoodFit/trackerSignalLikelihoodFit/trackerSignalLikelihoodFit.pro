TEMPLATE = app
TARGET = trackerSignalLikelihoodFit

CONFIG += debug

TOPLEVEL = ../../../..

INCLUDEPATH += \
  $$TOPLEVEL/widgets \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/likelihood \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/global \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis \
  $$TOPLEVEL/tools/postAnalysis/likelihoodFit

LIBS += \
  -L$$TOPLEVEL/lib \
  -lwidgets \
  -llikelihood \
  -lrootplot \
  -lglobal \
  -lparticle \
  -lprocessing \
  -lsetup \
  -lSimpleEvent \
  -lpostAnalysis \
  -llikelihood \
  -ltracking \
  -llikelihoodFit

CLASSES = \
  MainWindow \
  SignalHeightTrackerFitPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)