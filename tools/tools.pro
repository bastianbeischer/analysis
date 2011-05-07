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

macx {
  message("skipping parser on MacOS X")
  SUBDIRS-=parser
}
