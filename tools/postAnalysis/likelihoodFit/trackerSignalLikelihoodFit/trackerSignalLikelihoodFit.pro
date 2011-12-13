TEMPLATE = app
TARGET = trackerSignalLikelihoodFit

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
  -llikelihoodFit

CLASSES = \
  MainWindow

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
