TEMPLATE = subdirs

CONFIG  += ordered

SUBDIRS  = \
  alignment \
  eventdisplay \
  parser \
  preAnalysis \
  analysis \
  postAnalysis \
  presentation \
  tofAlignment \
	eventcounter

exists($(PERDAIX10PATH)) {
  SUBDIRS+= \
    parser \
    eventcounter
} else {
  message ("Set PERDAIX10PATH environment variable to build parser and eventcounter!")
}

macx {
  message("skipping parser on MacOS X")
  SUBDIRS-=parser
}
