DEPENDPATH += $$INCLUDEPATH

for(class, CLASSES) {
  HEADERS += $${class}.hh
  SOURCES += $${class}.cc
}

QMAKE_LFLAGS -= -Wl,--as-needed

# Seperate source & build dirs
DESTDIR = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR = ./.tmp
UI_DIR = ./.tmp
RCC_DIR = ./.tmp
