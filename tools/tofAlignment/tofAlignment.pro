TEMPLATE = app
TARGET = tofAlignment

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/alignment \
  $$TOPLEVEL/alignment/millepede \
  $$TOPLEVEL/event

LIBS += \
  -lgfortran \
  -L$$TOPLEVEL/lib \
  -lprocessing \
  -lSimpleEvent \
  -lsetup \
  -ltracking \
  -lmillepede

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
