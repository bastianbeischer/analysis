TEMPLATE = app
TARGET = presentation

TOPLEVEL = ../..

FORMS+= \
  MainWindow.ui

CLASSES+=\
  MainWindow

SOURCES = \
  main.cc

include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
