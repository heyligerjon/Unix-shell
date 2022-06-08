
cs1730bash: cs1730bash.o
	g++ -o cs1730bash cs1730bash.o

cs1730bash.o: cs1730bash.cpp
	g++ -c -Wall -std=c++14 -g -O0 -pedantic-errors cs1730bash.cpp

clean:
	rm *.o cs1730bash