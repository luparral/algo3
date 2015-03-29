all: medidor

medidor: medidor.o grasp.o greedy.o local1.o
	g++ -o medidor medidor.o grasp.o greedy.o local1.o

medidor.o: medidor.cpp
	c++ -c medidor.cpp

grasp.o: grasp.cpp
	g++ -c grasp.cpp

greedy.o: greedy.cpp
	g++ -c greedy.cpp

local1.o: local1.cpp
	g++ -c local1.cpp

clean:
	rm *o medidor
