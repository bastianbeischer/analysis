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

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
