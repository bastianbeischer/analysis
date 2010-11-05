TEMPLATE = lib
TARGET = setup

TOPLEVEL    = ..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

SOURCES += DetectorElement.cc \
           Layer.cc \
           Setup.cc \
           SipmArray.cc \
           TRDModule.cc \
           TOFBar.cc

HEADERS += DetectorElement.hh \
           Layer.hh \
           Setup.hh \
           SipmArray.hh \
           TRDModule.hh \
           TOFBar.hh

INCLUDEPATH += $$TOPLEVEL/event

DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent

include(../root.pri)