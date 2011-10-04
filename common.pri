DEPENDPATH += $$INCLUDEPATH

for(dir, SOURCEDIRS) {
  dir = $$OUT_PWD/$$dir
  !exists($${dir}) {
    error(Directory $${dir} does not exist!)
  }
  SOURCES += $$system(ls -1 $${dir}/*.cc)
  HEADERS += $$system(ls -1 $${dir}/*.hh)
  FORMS   += $$system(ls -1 $${dir}/*.ui 2>/dev/null)
  INCLUDEPATH += $$dir
}

for(class, CLASSES) {
  HEADERS += $${class}.hh
  SOURCES += $${class}.cc
}

QMAKE_LFLAGS += -Wl,--no-as-needed

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
