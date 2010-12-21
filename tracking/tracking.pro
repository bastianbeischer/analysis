TEMPLATE = lib
TARGET = tracking

CONFIG += debug

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += Track.cc \
           TrackInformation.cc \
           TrackFinding.cc \
           StraightLine.cc \
           BrokenLine.cc \
           CenteredBrokenLine.cc \
           CenteredBrokenLine2D.cc \
           Matrix.cc \
           StraightLineMatrix.cc \
           BrokenLineMatrix.cc \
           CenteredBrokenLineMatrix.cc \
           CenteredBrokenLineMatrix2D.cc

HEADERS += Track.hh \     
           TrackInformation.hh \
           TrackFinding.hh \
           StraightLine.hh \
           BrokenLine.hh \
           CenteredBrokenLine.hh \
           CenteredBrokenLine2D.hh \
           Matrix.hh \
           StraightLineMatrix.hh \
           BrokenLineMatrix.hh \
           CenteredBrokenLineMatrix.hh \
           CenteredBrokenLineMatrix2D.hh

INCLUDEPATH += $$TOPLEVEL/event $$TOPLEVEL/global

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent

include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)
