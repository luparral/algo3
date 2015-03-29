all: medidor

medidor: medidor.o greedy.o 
	g++ -o medidor medidor.o greedy.o 

medidor.o: medidor.cpp
	g++ -c medidor.cpp

greedy.o: greedy.cpp
	g++ -c greedy.cpp
	
clean: 
	rm *o medidor
