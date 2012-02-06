TEMPLATE = app
TARGET = likelihoodAnalysisToyMC

CONFIG += debug

TOPLEVEL = ../../../..

INCLUDEPATH += \
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/likelihood

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lsetup \
  -lSimpleEvent \
  -ltracking \
  -lprocessing \
  -lparticle \
  -llikelihood

SOURCES += \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
