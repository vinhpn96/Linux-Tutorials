IDIR =./inc
SDIR =./src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =./lib

LIBS=

_DEPS = cpu_affinity.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: cpu_affinity

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@if [ ! -d "$(ODIR)" ]; then \
		echo "create $(ODIR)"; \
		mkdir $(ODIR); \
	fi
	$(CC) -c -o $@ $< $(CFLAGS)

cpu_affinity: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: cpu_affinity clean

clean:
	rm -rf $(OBJ)
	rm -rf cpu_affinity
