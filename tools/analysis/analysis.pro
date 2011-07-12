TEMPLATE = app
TARGET = analysis

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lprocessing \
  -lparticle \
  -ltracking \
  -lrootplot \
  -lsetup

SOURCEDIRS = \
  main \
  tracker \
  tof \
  trd \
  reconstruction \
  generalPlots \
  mc \
  occupancy \
  slowControl \
  testbeam

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)

RESOURCES += \
  main/resources.qrc
