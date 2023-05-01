CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11 -O3 -fPIC
LDFLAGS = -shared
OBJS = libstamp.so

all: $(OBJS)

#%.o: %.cpp
#	$(CC) $(CFLAGS) -c $< -o $@

libstamp.so: stamp.h stamp.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) stamp.cpp -o libstamp.so

.PHONY: clean
clean:
	rm -f *.so