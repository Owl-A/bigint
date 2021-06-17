all: bigint.cpp main.cpp bigint.h
	g++ -g main.cpp bigint.cpp -o main
