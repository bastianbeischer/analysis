TEMPLATE = lib
TARGET = alignment

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += Constraint.cc \
           Manager.cc \
           Matrix.cc \
           Parameters.cc \
           Strategy.cc \
           TrackSelection.cc

HEADERS += Constraint.hh \
           Manager.hh \
           Matrix.hh \
           Parameters.hh \
           Strategy.hh \
           TrackSelection.hh

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/tracking \
                ./millepede

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -ltracking \
           -L./millepede -lmillepede

include(../root.pri)
