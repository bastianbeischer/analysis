ROOTINCDIR = $$system(${ROOTSYS}/bin/root-config --incdir)

!exists ($$ROOTINCDIR/TObject.h):error("Could NOT find ROOT!")

include($$ROOTINCDIR/rootcint.pri)
LIBS += -lHistPainter -lSpectrum -lRIO
