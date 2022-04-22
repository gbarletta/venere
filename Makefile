CC = g++
CFLAGS = -Wall -std=c++11 --pedantic

run: parser
	./parser

parser: parser.cpp
	g++ $(CFLAGS) parser.cpp -o parser