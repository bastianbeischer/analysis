TEMPLATE = lib
TARGET = SimpleEvent

CONFIG += debug

TOPLEVEL = ..

CLASSES += \

DEPENDPATH += $$INCLUDEPATH

CLASSES+= \
  Cluster \
  DataDescription \
  Hit \
  MCDigiSignal \
  MCEventInformation \
  MCSimpleEventDigi \
  MCSimpleEventParticle \
  SensorTypes \
  SimpleEvent \
  TOFCluster \
  TOFSipmHit

CREATE_ROOT_DICT_FOR_CLASSES += \
  Cluster.hh \
  DataDescription.hh \
  Hit.hh \
  MCDigiSignal.hh \
  MCEventInformation.hh \
  MCSimpleEventDigi.hh \
  MCSimpleEventParticle.hh \
  SensorTypes.hh \
  SimpleEvent.hh \
  TOFCluster.hh \
  TOFSipmHit.hh

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
