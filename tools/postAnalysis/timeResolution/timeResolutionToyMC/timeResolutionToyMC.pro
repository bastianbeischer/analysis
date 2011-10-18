TEMPLATE = app
TARGET = timeResolutionToyMC

CONFIG += debug

TOPLEVEL = ../../../..

INCLUDEPATH += \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal

SOURCES = \
  main.cc

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
