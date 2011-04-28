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
  CutFilter

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup \
  -lparticle \
  -ltracking

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
