ifeq ($(shell uname -s), Darwin)
CC = g++-9
else
CC = g++
endif
INCLUDE = -I../
CFLAGS = -std=c++17 -O2 -g -c ${INCLUDE}
OBJFILES = test.o

lint:
	clang-format -i -style=Google ../*.cpp ./*.cpp test.cpp

test: $(OBJFILES) 
	clang-format -i -style=Google ../*.cpp ./*.cpp test.cpp
	$(CC) -o test $(OBJFILES) 
	@python test_BigInteger.py > BigIntegero.txt 
	time ./test

test_big_int: $(OBJFILES) 
	clang-format -i -style=Google ../*.cpp ./*.cpp test.cpp
	$(CC) -o test $(OBJFILES) 
	@python test_BigInteger.py > BigIntegero.txt 
	time ./test big_int

test_wo_big_int: $(OBJFILES) 
	clang-format -i -style=Google ../*.cpp ./*.cpp test.cpp
	$(CC) -o test $(OBJFILES) 
	time ./test wo_big_int

test_tsp: tsp.o
	$(CC) -o tsp tsp.o
	./tsp

test_fenwick: fenwick.o
	$(CC) -o fenwick fenwick.o
	./fenwick

test_sudoku: test_sudoku.o
	$(CC) -o test_sudoku test_sudoku.o
	./test_sudoku

clean:
	rm -rf *.o
	rm -rf test

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp

tsp.o:  ../tsp.cpp
	$(CC) -std=c++11 -O2 -g -c ../tsp.cpp

test_sudoku.o: test_sudoku.cpp
	$(CC) $(CFLAGS) -std=c++11 -O2 -g -c test_sudoku.cpp

fenwick.o:  ../tsp.cpp
	$(CC) -std=c++11 -O2 -g -c ../fenwick.cpp
