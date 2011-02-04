DEPENDPATH += $$INCLUDEPATH

for(class, CLASSES) {
  HEADERS += $${class}.hh
  SOURCES += $${class}.cc
}

QMAKE_LFLAGS -= -Wl,--as-needed

# Seperate source & build dirs

contains(TEMPLATE, app) {
  DESTDIR = $$TOPLEVEL/bin
}
contains(TEMPLATE, lib) {
  DESTDIR = $$TOPLEVEL/lib
}
OBJECTS_DIR = ./.tmp
MOC_DIR = ./.tmp
UI_DIR = ./.tmp
RCC_DIR = ./.tmp
