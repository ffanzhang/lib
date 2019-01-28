CC = g++
INCLUDE = -I../
CFLAGS = -std=c++11 -g -c ${INCLUDE}
OBJFILES = test.o

test: $(OBJFILES) 
	clang-format -i -style=Google ../*.cpp test.cpp
	$(CC) -o test $(OBJFILES) 
	time ./test

clean:
	rm -rf *.o
	rm -rf test

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp
