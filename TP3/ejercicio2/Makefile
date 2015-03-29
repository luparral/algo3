all: main

main: ejercicio2.o greedy.o 
	g++ -o main ejercicio2.o greedy.o 

main.o: ejercicio2.cpp
	g++ -c ejercicio2.cpp

greedy.o: greedy.cpp
	g++ -c greedy.cpp
	
clean: 
	rm *o main
