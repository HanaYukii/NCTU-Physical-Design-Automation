all: Lab3

CXX = g++

OFLAGS = -O3


Lab3: Lab3-bstar.o 
	$(CXX) $(OFLAGS) Lab3-bstar.o -o Lab3

Lab3-bstar.o: Lab3-bstar.cpp
	$(CXX) $(OFLAGS) Lab3-bstar.cpp -c

clean:
	rm -rf *.o *.gch Lab3
