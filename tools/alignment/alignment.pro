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
  -lparticle \
  -llikelihood \
  -lmillepede

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
