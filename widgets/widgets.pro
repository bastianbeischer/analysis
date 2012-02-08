TEMPLATE = lib
TARGET = widgets

CONFIG += debug

TOPLEVEL = ..

CLASSES = \
  ParameterWidget \
  DoubleEdit

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
