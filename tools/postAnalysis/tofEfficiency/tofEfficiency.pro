TEMPLATE = app
TARGET = tofEfficiency

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -ltracking \
  -lrootplot \
  -lsetup \
  -lglobal \
  -lpostAnalysis

CLASSES = \
  MainWindow \
  BarEfficiencyPlot \
  MeanBarEfficiencyPlot

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
