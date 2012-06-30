TEMPLATE = subdirs

SUBDIRS  = \
  alignment \
  eventdisplay \
  analysis \
  presentation \
  decompressor \
  calibrationViewer

exists($(PERDAIX_READOUT_PATH)) {
  SUBDIRS+= \
    parser \
    eventcounter \
    sensorconverter
} else {
  message ("Set PERDAIX_READOUT_PATH environment variable to build parser and eventcounter!")
}

