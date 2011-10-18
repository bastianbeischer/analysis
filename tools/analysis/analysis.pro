TEMPLATE = app
TARGET = analysis

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/trdAnalysis \
  $$TOPLEVEL/fluxAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lprocessing \
  -lparticle \
  -ltracking \
  -lrootplot \
  -lsetup \
  -ltrdAnalysis \
  -lfluxAnalysis

SOURCEDIRS = .

RESOURCES += \
  resources.qrc


include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
