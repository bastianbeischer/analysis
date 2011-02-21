TEMPLATE = lib
TARGET = alignment

CONFIG += debug

TOPLEVEL = ..

CLASSES += \
  Constraint \
  DataInterface \
  Manager \
  AlignmentMatrix \
  Parameters \
  Strategy

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/processing \
  ./millepede

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup \
  -ltracking \
  -L./millepede \
  -lmillepede \
  -lprocessing \
  -lgfortran

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
