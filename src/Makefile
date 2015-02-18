######################################################################

VERNUM = 1.0

CC=gcc

# path to include (.h) files
INCDIR=../include
INCLUDE=graph.h
INCLUDES=$(patsubst %,$(INCDIR)/%,$(INCLUDE))


# path to bin dir where executable will end up
BINDIR=../bin/

# path to directory for temporary object file storage
OBJDIR=./obj/

# path to added libraries
LIBSDIR=../lib/
# lm = math library
LIBS=-lm

# compiler flags
CFLAGS=-I$(INCDIR) -g -o

# source files
SRCS=$(shell find ./ -maxdepth 1 -name "*.c" | sed 's!.*/!!')

OBJS  = $(addprefix $(OBJDIR), \
        $(patsubst %.c, %.o, $(SRCS)))

$(OBJDIR)%.o : %.c $(INCLUDES)
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) $@ $<

test: $(OBJS) main/test.c
	$(CC) $(CFLAGS) $(BINDIR)test-$(VERNUM) $^ $(LIBS)

gn: $(OBJS) main/gn.c
	$(CC) $(CFLAGS) $(BINDIR)gn-$(VERNUM) $^ $(LIBS)


.PHONY: clean

clean:
	rm -f $(OBJDIR)*.o
