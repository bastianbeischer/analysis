TEMPLATE = app
TARGET = timeShift

CONFIG += debug

TOPLEVEL = ../../..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/bin
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

FORMS+= MainWindow.ui

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/rootplot \
               $$TOPLEVEL/setup \
               $$TOPLEVEL/tracking

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -ltracking \
                         -lrootplot \
                         -lsetup

HEADERS = \
  MainWindow.hh \
  PostAnalysisPlot.hh \
  TimeDifferenceHistogram.hh

SOURCES = \
  main.cc \
  MainWindow.cc \
  PostAnalysisPlot.cc \
  TimeDifferenceHistogram.cc

include($$TOPLEVEL/root.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
