TEMPLATE = app
TARGET = fluxAnalysis

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/widgets \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lparticle \
  -ltracking \
  -lrootplot \
  -lsetup \
  -lprocessing \
  -lglobal \
  -lpostAnalysis \
  -llikelihood \
  -lwidgets

CLASSES = \
  MainWindow \
  RigiditySpectrumPlot \
  FluxCalculation

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
