TEMPLATE = lib
TARGET = rootplot

CONFIG += debug

TOPLEVEL = ..

INCLUDEPATH+= \
  $$TOPLEVEL/global

CLASSES += \
  RootPlot \
  H1DPlot \
  H2DPlot \
  GraphPlot \
  PerdaixDisplay

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
