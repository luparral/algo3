all: medidor

medidor: medidor.o greedy.o local1.o
	g++ -o medidor medidor.o greedy.o local1.o

medidor.o: medidor.cpp
	g++ -c medidor.cpp

greedy.o: greedy.cpp
	g++ -c greedy.cpp

local1.o: local1.cpp
	g++ -c local1.cpp

clean:
	rm *o medidor
