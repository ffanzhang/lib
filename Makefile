CC=g++
CFLAGS=-g -std=c++11

test: test.cpp
	$(CC) $(CFLAGS) -o test test.cpp
	./test
clean:
	rm -rf *.o
	rm -rf test
