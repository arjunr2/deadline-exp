CC = gcc
CFLAGS = -pthread
SRC_C := $(filter-out dd.c, $(wildcard *.c))
SRC_O := $(SRC_C:.c=.o)

all: $(SRC_O)

%.o: %.c dd.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm *.o
