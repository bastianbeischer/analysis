TEMPLATE = app
TARGET = decompressor

CONFIG += debug

TOPLEVEL = ../..

LIBS += \
  -L$$TOPLEVEL/lib \
  -lSimpleEvent

SOURCES += \
  main.cc

macx {
  QMAKE_CXXFLAGS += -DNO_QUICKUSB
  QMAKE_CXXFLAGS += -D__macx__
  QMAKE_CFLAGS += -DNO_QUICKUSB
  QMAKE_CFLAGS += -D__macx__
}

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
include($$TOPLEVEL/common.pri)
