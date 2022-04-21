CC = g++
CFLAGS = -Wall -std=c++11 --pedantic

parser: parser.cpp
	g++ $(CFLAGS) parser.cpp -o parser