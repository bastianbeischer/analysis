TEMPLATE = app
TARGET = timeOverThresholdScaling

CONFIG += debug

TOPLEVEL = ../../..

FORMS+= \
  MainWindow.ui

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -ltracking \
  -lrootplot \
  -lsetup \
  -lprocessing

CLASSES = \
  MainWindow \
  PostAnalysisPlot \
  timeOverThresholdScaling

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
