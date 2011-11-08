TEMPLATE = app
TARGET = signalHeightScaling

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
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lparticle \
  -ltracking \
  -lrootplot \
  -lsetup \
  -lprocessing \
  -lglobal \
  -lpostAnalysis

CLASSES = \
  MainWindow \
  SignalHeightProjection \
  SignalHeightProjectionCollection \
  SignalHeightTempCor \
  SignalHeightTempCorCollection \
  SignalHeightTimeCor \
  SignalHeightTimeCorCollection \
  SignalHeightTimeCor2D \
  SignalHeightTimeCor2DCollection

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
