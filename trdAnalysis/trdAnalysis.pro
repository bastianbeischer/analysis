TEMPLATE = lib
TARGET = trdAnalysis

CONFIG += debug

TOPLEVEL = ../

CLASSES += \
  TRDElectronIdentifierCutBased \
  TRDElectronIdentifierLikelihood \
  TRDLikelihoods

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

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
