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
  $$TOPLEVEL/event

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
