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
  ProcessingThread

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup \
  -ltracking

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
