TEMPLATE = lib
TARGET = rootplot

CONFIG += debug

TOPLEVEL = ..

INCLUDEPATH+= \
  $$TOPLEVEL/global

CLASSES += \
  RootPlot \
  RootQtWidget \
  H1DPlot \
  H2DPlot \
  H2DProjectionPlot \
  ProjectionControlWidget \
  StringSpinBox \
  DetectorSelectionWidget \
  TOFChannelSelectionWidget \
  GraphPlot \
  PerdaixDisplay \
  H3DPlot

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
