FLAGS = -std=c++11 -g -Wall -pedantic -Wextra -Wmissing-declarations

all : main.out

main.out: main.cpp
	g++ $(FLAGS) main.cpp -o main.out

clean: 
	rm -rf core *.o
	rm -rf core *.out
