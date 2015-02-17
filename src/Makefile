######################################################################

VERNUM = 1.0

CC=gcc

# path to include (.h) files
INCDIR=../include
INCLUDE=graph.h
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
MWOBJ=test.o util.o edges.o graph.o bfs.o queue.o vector.o metrics.o modularity.o


%.o: %.c $(INCLUDES)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(BINDIR)/test-$(VERNUM): $(MWOBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o
