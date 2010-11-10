TEMPLATE = lib
TARGET = tracking

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += Track.cc \
           TrackFinding.cc \
           StraightLine.cc \
           BrokenLine.cc \


HEADERS += Track.hh \     
           TrackFinding.hh \
           StraightLine.hh \
           BrokenLine.hh

INCLUDEPATH += $$TOPLEVEL/event

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent

include(../root.pri)
