TEMPLATE = lib
TARGET = trdAnalysis

CONFIG += debug

TOPLEVEL    = ../

# Seperate source & build dirs
DESTDIR     = $$TOPLEVEL/lib
OBJECTS_DIR = ./.tmp
MOC_DIR     = ./.tmp
UI_DIR      = ./.tmp
RCC_DIR     = ./.tmp

CLASSES += TRDElectronIdentifier \
           TRDElectronIdentifierCutBased \
           TRDElectronIdentifierLikelihood \
           TRDLikelihoods
           #TRDLikelihood


INCLUDEPATH += $$TOPLEVEL/event \
               $$TOPLEVEL/particle \
               $$TOPLEVEL/tracking

LIBS += -L$$TOPLEVEL/lib -lSimpleEvent \
                         -lparticle \
                         -ltracking


include($$TOPLEVEL/common.pri)
include($$TOPLEVEL/root.pri)
include($$TOPLEVEL/macx.pri)

QMAKE_LFLAGS -= -Wl,--as-needed
