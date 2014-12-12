CC      = gcc
CFLAGS  = -O2

all: makecontents

makecontents: makecontents.o
	$(CC) $(LDFLAGS) -o $@ $^

makecontents.o: makecontents.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f makecontents makecontents.o

.PHONY: clean