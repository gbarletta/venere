CC = g++
CFLAGS = -Wall --pedantic

parser: parser.cpp
	g++ $(CFLAGS) parser.cpp -o parser