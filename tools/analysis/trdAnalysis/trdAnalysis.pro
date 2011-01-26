TEMPLATE = lib
TARGET = trdAnalysis

CONFIG += debug

TOPLEVEL    = ../../../

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

HEADERS += TRDCalculations.hh \
           TRDLikelihood.hh \
           ProtonRejector.hh

SOURCES += TRDCalculations.cc \
           TRDLikelihood.cc \
           ProtonRejector.cc

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/global \
               $$TOPLEVEL/processing \
               $$TOPLEVEL/setup \
               $$TOPLEVEL/tracking

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -lprocessing \
                         -ltracking \
                         -lsetup


include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
