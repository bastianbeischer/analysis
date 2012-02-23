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
  $$TOPLEVEL/fluxAnalysis \
  $$TOPLEVEL/likelihood \
  $$TOPLEVEL/widgets

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lprocessing \
  -lparticle \
  -llikelihood \
  -ltracking \
  -lrootplot \
  -lsetup \
  -ltrdAnalysis \
  -lfluxAnalysis \
  -lWidgets

SOURCEDIRS = .

RESOURCES += \
  resources.qrc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
