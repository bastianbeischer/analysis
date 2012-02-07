macx {
    message("Building for MacOS X")
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
    contains(TEMPLATE, app):
        CONFIG -= app_bundle

    CONFIG += x86_64
    QMAKE_CXXFLAGS += -march=core2 -mtune=core2 -O2 -pipe
    QMAKE_LFLAGS -= -Wl,--no-as-needed
    QMAKE_LFLAGS -= -Wl,--as-needed
}
