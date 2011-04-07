TEMPLATE = lib
TARGET = particle

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  Particle

INCLUDEPATH += \
  $$TOPLEVEL/global \
  $$TOPLEVEL/tracking

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
