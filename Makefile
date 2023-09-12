
CC = gcc
CFLAGS = -O0 -g
TARGET = main

all: clean $(TARGET)

clean:
	find . -name '*.o' -delete

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

lib.o: lib.c
	$(CC) $(CFLAGS) -c lib.c -o lib.o

reader.o: reader.c
	$(CC) $(CFLAGS) -c reader.c -o reader.o

$(TARGET): main.o lib.o reader.o
	$(CC) $(CFLAGS) -o main main.o lib.o reader.o

check:
	valgrind --leak-check=full ./main 

.PHONY: all clean check
