TEMPLATE = app
TARGET = likelihood

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/likelihood \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/global \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -llikelihood \
  -lrootplot \
  -lglobal \
  -lparticle \
  -lpostAnalysis

CLASSES = \
  MainWindow \
  LikelihoodPDFPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
