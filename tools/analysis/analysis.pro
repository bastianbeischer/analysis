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
  Framework \
  GeneralPlots \
  MC \
  Occupancy \
  Reconstruction \
  SlowControl \
  TOF \
  TRD \
  Testbeam \
  Tracker \

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)

RESOURCES += \
  Framework/resources.qrc
