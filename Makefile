CC=g++
CFLAGS=-g -std=c++11

test: test.cpp
	clang-format -i -style=Google *.cpp
	$(CC) $(CFLAGS) -o test test.cpp
	time ./test
clean:
	rm -rf *.o
	rm -rf test
