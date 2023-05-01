CC = g++
CFLAGS = -Wall -g
LDFLAGS = -L../library -I../library
LD_LIBRARY_PATH = ../library
OBJS = fib.o matrix.o vector.o

all: $(OBJS)

#%.o: %.cpp
#	$(CC) $(CFLAGS) -c $< -o $@

fib.o: fib.cpp
	$(CC) $(CFLAGS) -c fib.cpp -o fib.o $(LDFLAGS) -lstamp

matrix.o: matrix.cpp
	$(CC) $(CFLAGS) -c matrix.cpp -o matrix.o $(LDFLAGS) -lstamp

vector.o: vector.cpp
	$(CC) $(CFLAGS) -c vector.cpp -o vector.o $(LDFLAGS) -lstamp


#g++ -Wall -g fib.o -o run -L../library -I../library -lstamp
run-fib: fib.o
	$(CC) $(CFLAGS) fib.o -o fib $(LDFLAGS) -lstamp
	./fib

#g++ -Wall -g matrix.o -o run -L../library -I../library -lstamp
run-matrix: matrix.o
	$(CC) $(CFLAGS) matrix.o -o matrix $(LDFLAGS) -lstamp
	./matrix

#g++ -Wall -g vector.o -o run -L../library -I../library -lstamp
run-vector: vector.o
	$(CC) $(CFLAGS) vector.o -o vector $(LDFLAGS) -lstamp
	./vector

.PHONY: clean
clean:
	rm -f *.o
	rm -f fib matrix vector