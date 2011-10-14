TEMPLATE = lib
TARGET = likelihood

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  Likelihood \
  LikelihoodPDF \
  LikelihoodRatio \
  TimeOfFlightLikelihood

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lparticle \
  -ltracking

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
