TEMPLATE = lib
TARGET = SimpleEvent

CONFIG += debug

TOPLEVEL = ..

CLASSES += \

DEPENDPATH += $$INCLUDEPATH

CLASSES+= \
  Cluster \
  CalibrationCollection \
  DataDescription \
  Hit \
  MCDigiSignal \
  MCEventInformation \
  MCSimpleEventDigi \
  MCSimpleEventParticle \
  SensorTypes \
  SimpleEvent \
  TOFCluster \
  TOFSipmHit \
  AdditionalHit

CREATE_ROOT_DICT_FOR_CLASSES += \
  Cluster.hh \
  CalibrationCollection.hh \
  DataDescription.hh \
  Hit.hh \
  MCDigiSignal.hh \
  MCEventInformation.hh \
  MCSimpleEventDigi.hh \
  MCSimpleEventParticle.hh \
  SensorTypes.hh \
  SimpleEvent.hh \
  TOFCluster.hh \
  TOFSipmHit.hh \
  AdditionalHit.hh

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
