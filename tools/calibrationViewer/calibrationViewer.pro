TEMPLATE = app
TARGET = calibrationViewer

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
  $$TOPLEVEL/global \
  $$TOPLEVEL/likelihood

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lprocessing \
  -lrootplot \
  -lparticle \
  -ltracking \
  -lsetup \
  -lglobal \
  -llikelihood

CLASSES+= \
  MainWindow

SOURCES += \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
