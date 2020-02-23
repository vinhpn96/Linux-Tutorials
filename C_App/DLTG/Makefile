IDIR =./inc
SDIR =./src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =./lib

LIBS=

_DEPS = dltg_main.h gtp.h #server.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = dltg_main.o gtp.o #server.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: dltg_out

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dltg_out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: dltg_out clean

clean:
	rm -rf $(OBJ)
	rm -rf dltg_out