CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11 -O3 -fPIC
LDFLAGS = -shared
OBJS = lamport.so

all: $(OBJS)

#%.o: %.cpp
#	$(CC) $(CFLAGS) -c $< -o $@

lamport.so: lamport_clock_simulator.h lamport_clock_simulator.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) lamport_clock_simulator.cpp -o liblamport.so

.PHONY: clean
clean:
	rm -f *.so