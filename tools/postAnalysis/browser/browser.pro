TEMPLATE = app
TARGET = browser

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/global \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/tools/postAnalysis/postAnalysis

LIBS += \
  -L$$TOPLEVEL/lib \
  -lrootplot \
  -lpostAnalysis \
  -lglobal

HEADERS = \
  MainWindow.hh

SOURCES = \
  main.cc \
  MainWindow.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
