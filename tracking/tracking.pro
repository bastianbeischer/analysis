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
           Matrix.cc \
           StraightLineMatrix.cc \
           BrokenLineMatrix.cc \
           CenteredBrokenLineMatrix.cc

HEADERS += Track.hh \     
           TrackInformation.hh \
           TrackFinding.hh \
           StraightLine.hh \
           BrokenLine.hh \
           CenteredBrokenLine.hh \
           Matrix.hh \
           StraightLineMatrix.hh \
           BrokenLineMatrix.hh \
           CenteredBrokenLineMatrix.hh

INCLUDEPATH += $$TOPLEVEL/event $$TOPLEVEL/global

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent

include(../root.pri)
