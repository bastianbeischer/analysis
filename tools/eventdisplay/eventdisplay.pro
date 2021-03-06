TEMPLATE = app
TARGET = eventdisplay

CONFIG += debug

TOPLEVEL = ../..

FORMS+= \
  MainWindow.ui

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/processing \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/particle \
  $$TOPLEVEL/tracking \
  $$TOPLEVEL/setup \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lprocessing \
  -lrootplot \
  -lparticle \
  -ltracking \
  -lsetup \
  -lglobal

CLASSES+= \
  Plotter \
  HitsPlot \
  MainWindow

SOURCES += \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
