IDIR=.
CC=g++
CCFLAGS=-I$(IDIR) -Wall
TARGET=test

ODIR=.

LIBS=-lm -lrt

HEADERS=$(IDIR)/*.hpp

SOURCES=test.cpp

OBJS=$(SOURCES:.c=.o)
OBJ=$(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CCFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ 
