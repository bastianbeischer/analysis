TEMPLATE = app
TARGET = parser

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/global \
  $$TOPLEVEL/setup

LIBS += \
  -L$$TOPLEVEL/lib \
  -lglobal \
  -lSimpleEvent \
  -lglobal \
  -lsetup

CLASSES = \
  Converter \
  DataManager \
  SingleFile \
  MCSingleFile \
  SensorsData \
  PreAnalysis

SOURCES += \
  main.cc


macx {
  QMAKE_CXXFLAGS += -DNO_QUICKUSB
  QMAKE_CXXFLAGS += -D__macx__
  QMAKE_CFLAGS += -DNO_QUICKUSB
  QMAKE_CFLAGS += -D__macx__
}

include(perdaix.pri)
include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
