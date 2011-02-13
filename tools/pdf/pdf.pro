TEMPLATE = app
TARGET = pdf

TOPLEVEL = ../..

FORMS+= \
  MainWindow.ui

CLASSES+=\
  MainWindow

SOURCES = \
  main.cc

include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
