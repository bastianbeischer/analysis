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
  ParticleProperties \
  TimeOfFlight

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/trdAnalysis


DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib \
        -lSimpleEvent \
        -ltracking

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
