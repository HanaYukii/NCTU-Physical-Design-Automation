all: Lab2

CXX = g++

OFLAGS = -O3


Lab2: main.o 
	$(CXX) $(OFLAGS) main.o -o Lab2

main.o: main.cpp
	$(CXX) $(OFLAGS) main.cpp -c

clean:
	rm -rf *.o *.gch Lab1
