TEMPLATE = app
TARGET = trdMoPV

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
  MoPVScaling \
  MoPVConstant

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
