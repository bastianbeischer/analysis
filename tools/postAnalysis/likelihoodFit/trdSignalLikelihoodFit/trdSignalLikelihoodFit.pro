TEMPLATE = app
TARGET = trdSignalLikelihoodFit

CONFIG += debug

TOPLEVEL = ../../../..

INCLUDEPATH += \
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
  -llikelihood \
  -lrootplot \
  -lglobal \
  -lparticle \
  -lprocessing \
  -lsetup \
  -lSimpleEvent \
  -lpostAnalysis \
  -ltracking \
  -llikelihood \
  -llikelihoodFit

CLASSES = \
  MainWindow \
  SignalHeightTrdFitPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
