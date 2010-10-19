TOPLEVEL     := ../..
BINDIR       := $(TOPLEVEL)/bin
TARGET       := $(BINDIR)/residuals

SOURCEDIR    := src
INCLUDEDIR   := include
TEMPDIR      := .tmp

MAIN         := main.cc

SOURCES      := DataChain.cc ResidualPlot.cc PlotHits.cc
HEADERS      := DataChain.hh ResidualPlot.hh PlotHits.hh

OBJECTS      := $(TEMPDIR)/$(MAIN:.cc=.o) $(addprefix $(TEMPDIR)/,$(SOURCES:.cc=.o))

# Internal
INCLUDEFLAGS := -I$(INCLUDEDIR)

# ROOT
INCLUDEFLAGS += -I$(shell root-config --incdir)
LIBS         += $(shell root-config --libs) -lHistPainter

# Other libraries
INCLUDEFLAGS += -I$(TOPLEVEL)/event -I$(TOPLEVEL)/tracking -I$(TOPLEVEL)/setup
LIBS         += -L$(TOPLEVEL)/lib -lSimpleEvent -ltracking -lsetup

# Compilers and flags
CC           := gcc
CXX          := g++
FLAGS        := -Wall -g -fPIC $(INCLUDEFLAGS) -MMD
CCFLAGS      := $(FLAGS)
CXXFLAGS     := $(FLAGS)

.PHONY: all clean

all: $(TARGET)

clean:
	@echo "Cleaning up ..."
	@rm -fr $(TEMPDIR)
	@rm -f $(TARGET)

$(TARGET): $(TEMPDIR) $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(LIBS)

$(TEMPDIR):
	@if [ ! -d $(TEMPDIR) ]; then mkdir $(TEMPDIR); fi

$(TEMPDIR)/%.o: $(SOURCEDIR)/%.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TEMPDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Include automatically regenerated dependency files, if they exist
-include $(OBJECTS:.o=.d)
