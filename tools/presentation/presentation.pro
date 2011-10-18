TEMPLATE = app
TARGET = presentation

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal
 
FORMS+= \
  MainWindow.ui

CLASSES+=\
  MainWindow

SOURCES = \
  main.cc

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
