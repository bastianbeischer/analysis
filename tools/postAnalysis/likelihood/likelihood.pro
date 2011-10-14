TEMPLATE = app
TARGET = likelihood

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/likelihood \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/global \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -llikelihood \
  -lrootplot \
  -lglobal \
  -lpostAnalysis

CLASSES = \
  MainWindow \
  LikelihoodPDFPlot \
  LikelihoodRatioPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
