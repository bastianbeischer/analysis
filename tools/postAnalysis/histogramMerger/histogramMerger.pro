TEMPLATE = app
TARGET = histogramMerger

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/global \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lrootplot \
  -lpostAnalysis

HEADERS = \
  MainWindow.hh \
  MergedHistogram1D.hh \
  MergedHistogram2D.hh

SOURCES = \
  main.cc \
  MainWindow.cc \
  MergedHistogram1D.cc \
  MergedHistogram2D.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
