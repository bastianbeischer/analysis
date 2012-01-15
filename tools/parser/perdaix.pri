!exists($(PERDAIX10PATH)) {
    error(Please set PERDAIX10PATH environment variable!)
}

!exists($(CLHEP_BASE_DIR)) {
    error(Please set CLHEP_BASE_DIR environment variable!)
}

PERDAIX_LIB_DIR = $$(PERDAIX10PATH)/lib

INCLUDEPATH += \
	./include \
	$$(PERDAIX10PATH)/flightsoftware/libraries/dataformats \
	$$(PERDAIX10PATH)/flightsoftware/libraries/global \
	$$(PERDAIX10PATH)/flightsoftware/libraries/sipm \
	$$(PERDAIX10PATH)/flightsoftware/libraries/perdaixusb \
        $$(PERDAIX10PATH)/flightsoftware/libraries/tof \
        $$(CLHEP_BASE_DIR)/include

DEPENDPATH += $${INCLUDEPATH}

LIBS += -L$${PERDAIX_LIB_DIR} -lperdaixdataformats -lperdaixusb -lperdaixglobal -lperdaixsipm -lperdaixtof
