TEMPLATE = lib
TARGET = processing

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  Corrections \
  AnalysisProcessor \
  EventReader \
  EventProcessor \
  EventQueue \
  EventDestination \
  DataChain \
  LimitedEventQueue \
  ProcessingThread \
  Cut \
  CutFilter \
  MCFilter

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global \
  $$TOPLEVEL/trdAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup \
  -lparticle \
  -ltracking \
  -ltrdAnalysis

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
