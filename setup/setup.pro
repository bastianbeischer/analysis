TEMPLATE = lib
TARGET = setup

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  Axis \
  DetectorElement \
  Layer \
  Setup \
  SipmArray \
  TRDModule \
  TOFBar \
  Settings \
  SettingsManager \
  FieldManager \
  InhomField \
  MagneticField \
  UniformField

INCLUDEPATH += \
  $$TOPLEVEL/global \
  $$TOPLEVEL/event

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
