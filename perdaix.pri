!exists($(PERDAIX_READOUT_PATH)) {
    error(Please set PERDAIX_READOUT_PATH environment variable!)
}

!exists($(CLHEP_BASE_DIR)) {
    error(Please set CLHEP_BASE_DIR environment variable!)
}

PERDAIX_LIB_DIR = $(PERDAIX_READOUT_PATH)/lib

PERDAIX_VERSION = $$system("basename $(echo $PERDAIX_READOUT_PATH | tr '[:lower:]' '[:upper:]')")
message("Using PERDaix readout version: $${PERDAIX_VERSION}.")

DEFINES+= \
  $${PERDAIX_VERSION}

INCLUDEPATH += \
	./include \
	$(PERDAIX_READOUT_PATH)/flightsoftware/libraries/dataformats \
	$(PERDAIX_READOUT_PATH)/flightsoftware/libraries/global \
	$(PERDAIX_READOUT_PATH)/flightsoftware/libraries/sipm \
	$(PERDAIX_READOUT_PATH)/flightsoftware/libraries/perdaixusb \
	$(PERDAIX_READOUT_PATH)/flightsoftware/libraries/sensors \
  $(PERDAIX_READOUT_PATH)/flightsoftware/libraries/tof \
  $(CLHEP_BASE_DIR)/include

DEPENDPATH += $${INCLUDEPATH}

LIBS += -L$${PERDAIX_LIB_DIR} \
  -lperdaixdataformats \
  -lperdaixusb \
  -lperdaixglobal \
  -lperdaixsipm \
  -lperdaixtof \
  -lperdaixsensors
