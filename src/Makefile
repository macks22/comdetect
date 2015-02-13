######################################################################

VERNUM = 1.0

CC=gcc

# path to include (.h) files
INCDIR=../include
INCLUDE=comdetect.h
INCLUDES=$(patsubst %,$(INCDIR)/%,$(INCLUDE))

# path to added libraries
LIBSDIR=../lib
# lm = math library
LIBS=-lm

# path to bin dir where executable will end up
BINDIR=../bin

# compiler flags
CFLAGS=-I$(INCDIR)

# make w/ objects
MWOBJ=sparsematrix.o comdetect.o

%.o: %.c $(INCLUDES)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/comdetect$(VERNUM): $(MWOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o
