TARGET   = main
CXX      = gcc
CPPFLAGS = -Wall -ansi -pedantic -g
LIBS     = -I../src -I./seatest

.PHONY: default clean test

OBJECTS = httpuino.o seatest.o main.o
HEADERS = $(wildcard *.h) $(wildcard ../src/*.h)

default: $(TARGET)

test: clean default
	./$(TARGET)

debug: clean default
	sudo gdb ./$(TARGET)

valgrind: clean default
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes --dsymutil=yes ./$(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -rf main.dSYM/

seatest.o: seatest/seatest.c $(HEADERS)
	$(CXX) $(CPPFLAGS) $(LIBS) -c $< -o $@

httpuino.o: ../src/httpuino.c $(HEADERS)
	$(CXX) $(CPPFLAGS) $(LIBS) -c $< -o $@

main.o: main.c $(HEADERS)
	$(CXX) $(CPPFLAGS) $(LIBS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
