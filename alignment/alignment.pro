TEMPLATE = lib
TARGET = alignment

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += Manager.cc \
           Matrix.cc \
           Parameters.cc \
           Strategy.cc \
           Constraint.cc \

HEADERS += Manager.hh \
           Matrix.hh \
           Parameters.hh \
           Strategy.hh \
           Constraint.hh \

INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/tracking \
                ./millepede

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -ltracking \
           -L./millepede -lmillepede

include(../root.pri)
