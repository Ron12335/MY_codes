CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -O2
LDFLAGS = -pthread

OBJ = producer.o consumer.o main.o functions.o

.PHONY: main clean force_rebuild

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o main

force_rebuild:
	rm -f main $(OBJ)
	$(MAKE)
