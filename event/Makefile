TOPLEVEL    := ..

LIBDIR      := $(TOPLEVEL)/lib
TEMPDIR     := .tmp

NAME        := SimpleEvent
TARGET      := $(LIBDIR)/lib$(NAME).so

CLASSES = SimpleEvent MCEventInformation MCSimpleEventParticle Hit Cluster DataDescription TOFSipmHit TOFCluster SensorTypes

SOURCES = $(addsuffix .cc, $(CLASSES))
HEADERS = $(addsuffix .hh, $(CLASSES))
DICTS   = $(addprefix $(TEMPDIR)/,$(SOURCES:.cc=Dict.cc))
OBJECTS = $(addprefix $(TEMPDIR)/,$(SOURCES:.cc=.o)) $(DICTS:.cc=.o)

# Compilers and Flags
CXX          := g++
INCLUDEDIRS  := . $(shell root-config --incdir)
INCLUDEFLAGS := $(addprefix -I, $(INCLUDEDIRS))
LIBS         := $(shell root-config --libs)
CXXFLAGS     := -g -O2 -Wall -fPIC $(INCLUDEFLAGS) -MMD
LFLAGS       := $(shell if [ `uname` = "Darwin" ]; then echo "-install_name @rpath/lib$(NAME).so"; fi)

.PHONY: all clean
.SECONDARY: $(DICTS)

all: $(TEMPDIR) $(LIBDIR) $(TARGET)

clean:
	@echo "Cleaning up ..."
	@rm -fr $(TEMPDIR)

distclean:
	@echo "Cleaning up everything ..."
	@rm -fr $(TEMPDIR)
	@rm -f $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LFLAGS) -o $(TARGET) $(OBJECTS) -shared $(LIBS)

$(LIBDIR):
	@if [ ! -d $(LIBDIR) ]; then mkdir $(LIBDIR); fi

$(TEMPDIR):
	@if [ ! -d $(TEMPDIR) ]; then mkdir $(TEMPDIR); fi

$(TEMPDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TEMPDIR)/%Dict.o: $(TEMPDIR)/%Dict.cc

$(TEMPDIR)/%Dict.cc: %.hh %.cc
	rootcint -f $@ -c $(INCLUDEFLAGS) -p $<

# Include automatically regenerated dependency files, if they exist
-include $(OBJECTS:.o=.d)
