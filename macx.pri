macx {
    message("Building for MacOS X")
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    contains(TEMPLATE, app):
        CONFIG -= app_bundle

    contains(TEMPLATE, lib):
        QMAKE_LFLAGS_SONAME += -install_name @rpath/lib$${TARGET}.dylib
    
    LIBS += -Wl,-rpath,$${PWD}/lib

    CONFIG += x86_64
    QMAKE_CXXFLAGS += -march=core2 -mtune=core2 -O2 -pipe
}
