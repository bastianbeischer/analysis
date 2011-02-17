TEMPLATE = app
TARGET = preAnalysis

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup

SOURCES = \
  main.cc
                  
include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
