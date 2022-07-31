CC = g++
CFLAGS = -Wall -std=c++11 --pedantic -g

run: vc
	./vc

vc: main.cpp parser astnode token
	g++ -c $(CFLAGS) main.cpp -o main.o
	g++ $(CFLAGS) -o vc main.o parser.o astnode.o token.o

parser: parser.cpp
	g++ -c $(CFLAGS) parser.cpp -o parser.o

astnode: astnode.cpp
	g++ -c $(CFLAGS) astnode.cpp -o astnode.o

token: token.cpp
	g++ -c $(CFLAGS) token.cpp -o token.o

clean:
	rm -fr parser *.dSYM