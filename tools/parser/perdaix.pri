!exists($(PERDAIX10_PATH)) {
    error(Please set PERDAIX10_PATH environment variable!)
}

PERDAIX_LIB_DIR = $(PERDAIX10_PATH)/lib

CLHEPDIR = $(CLHEP_BASE_DIR)

INCLUDEPATH += \
	./include \
	$(PERDAIX10_PATH)/flightsoftware/libraries/dataformats \
	$(PERDAIX10_PATH)/flightsoftware/libraries/global \
	$(PERDAIX10_PATH)/flightsoftware/libraries/sipm \
	$(PERDAIX10_PATH)/flightsoftware/libraries/perdaixusb \
        $(PERDAIX10_PATH)/flightsoftware/libraries/tof \
        $(CLHEPDIR)/include

DEPENDPATH += $${INCLUDEPATH}

LIBS += -L$${PERDAIX_LIB_DIR} -lperdaixdataformats -lperdaixusb -lperdaixglobal -lperdaixsipm -lperdaixtof -lquickusb -lusb

linux-g++-64 {
  LIBS -= -lquickusb -lusb
}

macx {
  LIBS -= -lquickusb -lusb
}
