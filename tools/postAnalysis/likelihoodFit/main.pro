TEMPLATE = lib
TARGET = likelihoodFit

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/likelihood \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/global \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -llikelihood \
  -lrootplot \
  -lglobal \
  -lparticle \
  -lprocessing \
  -lsetup \
  -lSimpleEvent \
  -lpostAnalysis

CLASSES = \
  LikelihoodFitWindow \
  LikelihoodPDFPlot \
  ParameterPlot \
  ParameterWidget \
  LikelihoodPDFFitPlot \
  DoubleEdit \
  NormalizationThread

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
