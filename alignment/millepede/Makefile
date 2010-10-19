LIB = libmillepede.so

FC = gfortran
LD = gfortran

FFLAGS += -fPIC

all: $(LIB)

clean:
	@echo "Cleaning up millepede ..."
	@rm -f *.o $(LIB)

$(LIB): millepede.o
	$(LD) -o $(LIB) millepede.o -shared
