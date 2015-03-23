TARGET   = main
CC       = gcc
INCLUDES = -I../src -I./seatest
CFLAGS   = -Wall -pedantic
LDFLAGS  =

.PHONY: default clean test debug valgrind
.PRECIOUS: $(TARGET) $(OBJECTS)

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LDFLAGS  = -lbsd
endif

ifeq ($(debug),1)
CFLAGS += -g -DDEBUG=1
endif

OBJECTS = httpuino.o seatest.o main.o
HEADERS = $(wildcard *.h) $(wildcard ../src/*.h)

default: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -rf main.dSYM/

test: clean default
	./$(TARGET)

debug: clean default
	sudo gdb ./$(TARGET)

valgrind: clean default
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes --dsymutil=yes ./$(TARGET)

seatest.o: seatest/seatest.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

httpuino.o: ../src/httpuino.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
