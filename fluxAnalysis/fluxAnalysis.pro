TEMPLATE = lib
TARGET = fluxAnalysis

CONFIG += debug

TOPLEVEL = ..

INCLUDEPATH+= \
  $$TOPLEVEL/global \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/event

SOURCEDIRS += .

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lparticle \
  -lsetup \
  -lrootplot \
  -lprocessing

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)

CLASSES -= SimulationFluxRatioWidget
