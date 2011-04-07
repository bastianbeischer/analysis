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
  TrackInformation

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
