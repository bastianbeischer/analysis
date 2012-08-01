TEMPLATE = app
TARGET = projector

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/global \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/widgets \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lrootplot \
  -lpostAnalysis \
  -lwidgets

HEADERS = \
  MainWindow.hh \
  ProjectionPlot.hh

SOURCES = \
  main.cc \
  MainWindow.cc \
  ProjectionPlot.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
