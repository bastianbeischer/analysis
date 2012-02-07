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
  MCFilter \
  AnalysisProcessorSetting

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/global \
  $$TOPLEVEL/likelihood

macx {
  LIBS += \
    -L$$TOPLEVEL/lib \
    -lglobal \
    -lSimpleEvent \
    -lsetup \
    -lparticle \
    -ltracking \
    -llikelihood
}

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
