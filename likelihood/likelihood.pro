TEMPLATE = lib
TARGET = likelihood

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  Likelihood \
  LikelihoodPDF \
  TimeOfFlightLikelihood \
  TrackerMomentumLikelihood \
  CherenkovLikelihood \
  LikelihoodAnalysis \
  Hypothesis

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/processing

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lparticle \
  -ltracking \
  -lsetup

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
