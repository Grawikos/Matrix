# List your *.h files (if you do not have them in your project then leave the variable "headers" empty):
headers = Matrix.h

# List your *.cpp files:
sources = TestMatrix.cpp Matrix.cpp

# Specify name of your program:
executable = TMatrix

$(executable): $(headers) $(sources)
	g++ -g -Wall -O0 -pedantic $(sources) -o $(executable)

.PHONY: clean
clean:
	rm $(executable)

.PHONY: check
check: $(executable)
	valgrind -s --leak-check=full --track-origins=yes ./$(executable)
