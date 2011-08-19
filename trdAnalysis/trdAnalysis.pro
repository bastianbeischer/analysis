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

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
