TOPLEVEL     := ../..
LIBDIR       := $(TOPLEVEL)/lib
TARGET       := $(LIBDIR)/libmillepede.so

SOURCEDIR    := .
INCLUDEDIR   := .
TEMPDIR      := .tmp

SOURCES      := millepede.f
HEADERS      := millepede.h
OBJECTS      := $(addprefix $(TEMPDIR)/,$(SOURCES:.f=.o))

# Internal
INCLUDEFLAGS := -I$(INCLUDEDIR)

# Compilers and flags
FC           := gfortran
FLAGS        := -g -fPIC $(INCLUDEFLAGS)
FCFLAGS      := $(FLAGS)

.PHONY: all clean

all: $(TARGET)

clean:
	@echo "Cleaning up ..."
	@rm -fr $(TEMPDIR)

distclean:
	@echo "Cleaning up everything..."
	@rm -fr $(TEMPDIR)
	@rm -f $(TARGET)

$(TARGET): $(TEMPDIR) $(OBJECTS)
	$(FC) -shared -o $(TARGET) $(OBJECTS) $(LIBS)

$(TEMPDIR):
	@if [ ! -d $(TEMPDIR) ]; then mkdir $(TEMPDIR); fi

$(TEMPDIR)/%.o: $(SOURCEDIR)/%.f
	$(FC) $(FCFLAGS) -o $@ -c $<

# Include automatically regenerated dependency files, if they exist
-include $(OBJECTS:.o=.d)
