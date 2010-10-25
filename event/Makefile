LIBDIR     := ../lib
TARGET     := $(LIBDIR)/libSimpleEvent.so

INCLUDEDIR := .
TEMPDIR    := .tmp

SOURCES = SimpleEvent.cc Hit.cc Cluster.cc DataDescription.cc DataChain.cc
HEADERS = SimpleEvent.hh Hit.hh Cluster.hh DataDescription.hh DataChain.hh
DICTS   = $(addprefix $(TEMPDIR)/,$(SOURCES:.cc=Dict.cc))
OBJECTS = $(addprefix $(TEMPDIR)/,$(SOURCES:.cc=.o)) $(DICTS:.cc=.o)

# Compilers and Flags
CC           := gcc
CXX          := g++
INCLUDEFLAGS := -I$(INCLUDEDIR) -I$(shell root-config --incdir)
LIBS         := $(shell root-config --libs)
CCFLAGS      := -Wall -fPIC $(INCLUDEFLAGS) -MMD
CXXFLAGS     := -Wall -fPIC $(INCLUDEFLAGS) -MMD

.PHONY: all clean
.SECONDARY: $(DICTS)

all: $(TARGET)

clean:
	@echo "Cleaning up ..."
	@rm -fr $(TEMPDIR)

distclean:
	@echo "Cleaning up everything ..."
	@rm -fr $(TEMPDIR)
	@rm -f $(TARGET)


$(TARGET): $(TEMPDIR) $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) -shared $(LIBS)

$(TEMPDIR):
	@if [ ! -d $(TEMPDIR) ]; then mkdir $(TEMPDIR); fi

$(TEMPDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TEMPDIR)/%Dict.o: $(TEMPDIR)/%Dict.cc

$(TEMPDIR)/%Dict.cc: %.hh %.cc
	rootcint -f $@ -c -p $<

# Include automatically regenerated dependency files, if they exist
-include $(OBJECTS:.o=.d)
