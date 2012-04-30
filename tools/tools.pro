TEMPLATE = subdirs

SUBDIRS  = \
  alignment \
  eventdisplay \
  analysis \
  postAnalysis \
  presentation \
  decompressor \
  calibrationViewer

exists($(PERDAIX_READOUT_PATH)) {
  SUBDIRS+= \
    parser \
    eventcounter
} else {
  message ("Set PERDAIX_READOUT_PATH environment variable to build parser and eventcounter!")
}

