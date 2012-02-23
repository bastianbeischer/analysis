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
  $$TOPLEVEL/event \
  $$TOPLEVEL/widgets

SOURCEDIRS += .

macx {
  LIBS += \
    -L$$TOPLEVEL/lib \
    -lglobal \
    -lparticle \
    -lrootplot \
    -lsetup \
    -lprocessing \
    -lwidgets
}

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
