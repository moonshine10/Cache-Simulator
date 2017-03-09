

output: Simulator.o block.o index.o cache.o
	g++ Simulator.o block.o index.o cache.o -o output


Simulator.o: Simulator.cpp
	g++ -c Simulator.cpp

	
cache.o: cache.cpp 
	g++ -c cache.cpp cache.hpp

index.o: index.cpp 
	g++ -c index.cpp index.hpp

block.o: block.cpp 
	g++ -c block.cpp block.hpp

clean:
	rm *.o output
