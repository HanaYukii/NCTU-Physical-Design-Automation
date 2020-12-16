all: Lab4

CXX = g++

OFLAGS = -O3


Lab4: main.o 
	$(CXX) $(OFLAGS) main.o -o Lab4

main.o: main.cpp
	$(CXX) $(OFLAGS) main.cpp -c

clean:
	rm -rf *.o *.gch Lab4
