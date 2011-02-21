TEMPLATE = lib
TARGET = tracking

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  Track \
  TrackInformation \
  TrackFinding \
  StraightLine \
  BrokenLine \
  CenteredBrokenLine \
  CenteredBrokenLine2D \
  Matrix \
  StraightLineMatrix \
  BrokenLineMatrix \
  CenteredBrokenLineMatrix \
  CenteredBrokenLineMatrix2D

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global

DEPENDPATH += $$INCLUDEPATH

macx {
  message("explicit linking needed for MacOS X")
  LIBS += \
    -L$$TOPLEVEL/lib \
    -lSimpleEvent
}

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
