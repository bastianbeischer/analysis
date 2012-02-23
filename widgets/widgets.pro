TEMPLATE = lib
TARGET = widgets

CONFIG += debug

TOPLEVEL = ..

CLASSES = \
  ParameterWidget \
  DoubleEdit \
  SelectionWidget

HEADERS+= \
  EnumSelector.hh

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
