all:
	g++ -Wall -ggdb -std=c++17 -Iinc -DDEBUG -o exe main.cpp inc/matrix.cpp inc/vector.cpp
