TEMPLATE = lib
TARGET = tracking

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  BrokenLine \
  BrokenLineMatrix \
  CenteredBrokenLine \
  CenteredBrokenLine2D \
  CenteredBrokenLineMatrix \
  CenteredBrokenLineMatrix2D \
  Matrix \
  StraightLine \
  StraightLineMatrix \
  Track \
  TrackFinding \
  TrdTrackFinding

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/setup

DEPENDPATH += $$INCLUDEPATH

macx {
  LIBS += \
    -L$$TOPLEVEL/lib \
    -lSimpleEvent \
    -lsetup \
    -lglobal
}

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
