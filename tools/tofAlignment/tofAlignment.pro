TEMPLATE = app
TARGET = tofAlignment

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/alignment \
  $$TOPLEVEL/alignment/millepede \
  $$TOPLEVEL/event \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/global

LIBS += \
  -lgfortran \
  -L$$TOPLEVEL/lib \
  -lprocessing \
  -lSimpleEvent \
  -lsetup \
  -ltracking \
  -lprocessing \
  -lmillepede

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
