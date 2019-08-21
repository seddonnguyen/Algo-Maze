all: solveMaze

clean:
	rm main.o solveMaze

solveMaze: main.o
	g++ main.o -o solveMaze

main.o: main.cpp
	g++ -c main.cpp
	
