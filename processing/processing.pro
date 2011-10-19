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
  $$TOPLEVEL/likelihood

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lsetup \
  -lparticle \
  -ltracking \
  -llikelihood

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
