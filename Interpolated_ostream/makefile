CPPFLAGS= -Wall -Wextra -pedantic -g -std=c++17

all: interpolate_bin

interpolate_bin: interpolate.o
	g++ $(CPPFLAGS) interpolate.o -o interpolate_bin

interpolate.o: Test_Interpolate.cpp Interpolate.hpp
	g++ $(CPPFLAGS) -c Test_Interpolate.cpp -o interpolate.o

run:
	# Insert command to run the executable
	./interpolate_bin
checkmem:
	# Insert command to run the executable with valgrind
	valgrind --tool=memcheck --leak-check=full ./interpolate_bin
clean:
	# Insert command to clean object files, executables
	rm ./interpolate_bin
