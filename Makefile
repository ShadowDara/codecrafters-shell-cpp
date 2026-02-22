# Makefile to build the program for Linux

all: run

run:
	g++ -std=c++23 main.cpp src/*.cpp -o shell
