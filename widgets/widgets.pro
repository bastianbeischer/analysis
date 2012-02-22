TEMPLATE = lib
TARGET = widgets

CONFIG += debug

TOPLEVEL = ..

CLASSES = \
  ParameterWidget \
  DoubleEdit \
  SelectionWidget \
  EnumSelector

INCLUDEPATH += \
  $$TOPLEVEL/global

DEPENDPATH += $$INCLUDEPATH

macx {
  LIBS += \
    -L$$TOPLEVEL/lib \
    -lglobal
}

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
