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
  DetectorIDSpinBox \
  DetectorSelectionWidget \
  GraphPlot \
  PerdaixDisplay

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
