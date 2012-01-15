TEMPLATE = app
TARGET = eventcounter

CONFIG += debug

TOPLEVEL = ../..

INCLUDEPATH += \
  $$TOPLEVEL/event \
  $$TOPLEVEL/setup

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent \
  -lsetup

SOURCES += \
  main.cc

QMAKE_CXXFLAGS += -DNO_QUICKUSB
QMAKE_CFLAGS += -DNO_QUICKUSB

macx {
  QMAKE_CXXFLAGS += -D__macx__
  QMAKE_CFLAGS += -D__macx__
}

include(perdaix.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
