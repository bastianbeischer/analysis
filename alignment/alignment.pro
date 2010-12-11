TEMPLATE = lib
TARGET = alignment

CONFIG += debug

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += Constraint.cc \
           DataInterface.cc \
           Manager.cc \
           AlignmentMatrix.cc \
           Parameters.cc \
           Strategy.cc

HEADERS += Constraint.hh \
           DataInterface.hh \
           Manager.hh \
           AlignmentMatrix.hh \
           Parameters.hh \
           Strategy.hh

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/setup \
               $$TOPLEVEL/tracking \
                ./millepede

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -lsetup \
                         -ltracking \
           -L./millepede -lmillepede \
                         -lgfortran

include(../root.pri)
