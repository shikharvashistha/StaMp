CC = g++
CFLAGS = -Wall -g
LDFLAGS = -L../library -I../library
LD_LIBRARY_PATH = ../library
OBJS = lamport.o

all: $(OBJS)

#%.o: %.cpp
#	$(CC) $(CFLAGS) -c $< -o $@

lamport.o: lamport.cpp
	$(CC) $(CFLAGS) -c lamport.cpp -o lamport.o $(LDFLAGS) -llamport


#g++ -Wall -g -c lamport.cpp -o lamport.o -L../library -I../library -llamport
run: lamport.o
	$(CC) $(CFLAGS) lamport.o -o lamport $(LDFLAGS) -llamport
	./lamport

.PHONY: clean
clean:
	rm -f *.o
	rm -f lamport