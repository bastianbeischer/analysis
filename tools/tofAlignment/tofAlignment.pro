TEMPLATE = app
TARGET = tofAlignment

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/alignment \
  $$TOPLEVEL/event \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/global

LIBS += \
  -lgfortran \
  -L$$TOPLEVEL/lib \
  -lparticle \
  -lprocessing \
  -lSimpleEvent \
  -lsetup \
  -ltracking \
  -lprocessing

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
