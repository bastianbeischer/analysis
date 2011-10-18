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
  -lglobal \
  -lrootplot \
  -lpostAnalysis

HEADERS = \
  MainWindow.hh

SOURCES = \
  main.cc \
  MainWindow.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
