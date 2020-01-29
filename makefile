FLAGS = -std=c++11 -g -Wall -pedantic -Wextra -Wmissing-declarations

all : main.out tests

main.out: main.cpp aes.cpp
	g++ $(FLAGS) main.cpp -o main.out

run: main.out
	valgrind --leak-check=full ./main.out

aes.o: aes.cpp
	g++ $(FLAGS) -c aes.cpp -o aes.o # the -c option says not to run the linker. Then the output consists of object files output by the assembler.

tests:  tests.out
	./tests/tests.out

tests.out: tests/tests.cpp
	g++ -std=c++11 tests/tests.cpp -o tests/tests.out
	
clean: 
	rm -rf core *.o
	rm -rf core *.out
