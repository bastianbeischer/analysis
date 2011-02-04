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
  $$TOPLEVEL/processing \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/tracking \
  ./millepede

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lprocessing \
  -lsetup \
  -ltracking \
  -L./millepede \
  -lmillepede \
  -lgfortran

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
