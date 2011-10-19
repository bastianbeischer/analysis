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
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/global \
  ./millepede

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup \
  -lparticle \
  -ltracking \
  -L./millepede \
  -lmillepede \
  -lprocessing \
  -lgfortran

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
