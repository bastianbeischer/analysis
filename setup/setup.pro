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
  InhomField \
  MagneticField \
  UniformField

INCLUDEPATH += \
  $$TOPLEVEL/global \
  $$TOPLEVEL/event

macx {
  LIBS += \
    -L$$TOPLEVEL/lib \
    -lglobal \
    -lSimpleEvent
}

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
