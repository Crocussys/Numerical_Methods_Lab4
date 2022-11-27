all: main.o
	g++ main.o -o gauss_seidel
	
main.o: main.cpp
	g++ -c main.cpp
	
clean:
	rm -rf *.o