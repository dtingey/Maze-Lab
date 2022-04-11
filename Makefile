CFLAGS=-std=c++11 -g
test: main.cpp Pathfinder.cpp Pathfinder.h PathfinderInterface.h
	g++ $(CFLAGS) -o solvemaze main.cpp Pathfinder.cpp