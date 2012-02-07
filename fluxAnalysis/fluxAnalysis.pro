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

macx {
  LIBS += \
    -L$$TOPLEVEL/lib \
    -lglobal \
    -lparticle \
    -lrootplot \
    -lsetup \
    -lprocessing
}

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
