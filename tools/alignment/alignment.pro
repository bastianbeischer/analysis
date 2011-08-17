TEMPLATE = app
TARGET = alignment

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/alignment

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lalignment \
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
