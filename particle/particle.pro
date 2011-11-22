TEMPLATE = lib
TARGET = particle

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  ParticleDB \
  Particle \
  ParticleFilter \
  ParticleIdentifier \
  ParticleInformation \
  TimeOfFlight \
  TRDReconstruction

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/likelihood \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/setup

DEPENDPATH += $$INCLUDEPATH

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -ltracking \
  -lsetup

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
