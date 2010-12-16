TEMPLATE = lib
TARGET = millepede
QT -= core gui

CONFIG += debug

TOPLEVEL    = ../..

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

F90_SOURCES += millepede.f
HEADERS += millepede.h

F90 = gfortran
F90_FLAGS = -fPIC

f90.output = ${OBJECTS_DIR}/${QMAKE_FILE_BASE}.o
f90.commands = $$F90 $$F90_FLAGS -c ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
f90.input = F90_SOURCES
QMAKE_EXTRA_COMPILERS += f90
