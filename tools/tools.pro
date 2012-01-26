TEMPLATE = subdirs

SUBDIRS  = \
  alignment \
  eventdisplay \
  preAnalysis \
  analysis \
  postAnalysis \
  presentation

exists($(PERDAIX10PATH)) {
  SUBDIRS+= \
    parser \
    eventcounter
} else {
  message ("Set PERDAIX10PATH environment variable to build parser and eventcounter!")
}

