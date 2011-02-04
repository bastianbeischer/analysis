TEMPLATE = lib
TARGET = setup

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  DetectorElement \
  Layer \
  Setup \
  SipmArray \
  TRDModule \
  TOFBar

INCLUDEPATH += \
  $$TOPLEVEL/event

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
