TEMPLATE = lib
TARGET = postAnalysis

CONFIG += debug

TOPLEVEL = ../../..

INCLUDEPATH += \
  $$TOPLEVEL/rootplot \
  $$TOPLEVEL/global

LIBS += \
  -L$$TOPLEVEL/lib \
  -lrootplot \
  -lglobal

SOURCEDIRS = .

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
