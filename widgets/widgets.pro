TEMPLATE = lib
TARGET = widgets

CONFIG += debug

TOPLEVEL = ..

CLASSES = \
  ParameterWidget \
  DoubleEdit \
  SelectionWidget \
  EnumSelector

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
