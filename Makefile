all:
	g++ -Wall -c src/utils.cpp src/webserver.cpp main.cpp
	g++ -Wall -o webserver utils.o webserver.o main.o
clean:
	rm -rf *.o webserver
