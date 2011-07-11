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
  TrackFinding

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/setup

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent -lsetup

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
