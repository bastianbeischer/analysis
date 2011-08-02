TEMPLATE = lib
TARGET = fluxAnalysis

CONFIG += debug

TOPLEVEL = ..

INCLUDEPATH+= \
  $$TOPLEVEL/global \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/event

SOURCEDIRS += .

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lparticle \
  -lrootplot \
  -lprocessing

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
