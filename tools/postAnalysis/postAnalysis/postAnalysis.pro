TEMPLATE = lib
TARGET = postAnalysis

CONFIG += debug

TOPLEVEL = ../../..

FORMS+= \
  PostAnalysisWindow.ui

INCLUDEPATH += \
  $$TOPLEVEL/rootplot

LIBS += \
  -L$$TOPLEVEL/lib \
  -lrootplot

CLASSES = \
  PostAnalysisWindow \
  PostAnalysisPlot

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
