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
  $$TOPLEVEL/global \
  $$TOPLEVEL/processing

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lparticle \
  -ltracking \
  -lprocessing

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
