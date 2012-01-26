TEMPLATE = lib
TARGET = millepede

CONFIG -= qt
CONFIG += debug

TOPLEVEL = ../..

F90_SOURCES += millepede.f
HEADERS += millepede.h

F90 = gfortran
F90_FLAGS = -fPIC

f90.output = ${OBJECTS_DIR}/${QMAKE_FILE_BASE}.o
f90.commands = $$F90 $$F90_FLAGS -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
f90.input = F90_SOURCES
QMAKE_EXTRA_COMPILERS += f90

macx {
  LIBS += -lgfortran
}

include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/macx.pri)
