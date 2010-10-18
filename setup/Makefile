TARGET     := libsetup.so

INCLUDEDIR := .
TEMPDIR    := .tmp

SOURCES = Layer.cc SiPMArray.cc
HEADERS = Layer.hh SiPMArray.hh 
OBJECTS = $(addprefix $(TEMPDIR)/,$(SOURCES:.cc=.o))

# Compilers and Flags
CC           := gcc
CXX          := g++
INCLUDEFLAGS := -I$(INCLUDEDIR) -I../event -I$(shell root-config --incdir)
LIBS         := $(shell root-config --libs)
CCFLAGS      := -Wall -fPIC $(INCLUDEFLAGS) -MMD
CXXFLAGS     := -Wall -fPIC $(INCLUDEFLAGS) -MMD

.PHONY: all clean

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

# Include automatically regenerated dependency files, if they exist
-include $(OBJECTS:.o=.d)
