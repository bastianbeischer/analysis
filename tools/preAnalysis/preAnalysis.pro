TEMPLATE = app
TARGET = preAnalysis

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/processing

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup \
  -lprocessing

SOURCES = \
  main.cc
                  
CLASSES = \
  PreAnalysisManager \
  ClusterGenerator \
  EventWriter

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
