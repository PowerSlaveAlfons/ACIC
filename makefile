all: PSAString

PSAString: alfons.o
	g++ -std=c++11 *.o -o PSAStringTester -fsanitize=leak

clang: alfons.cpp
	clang++ alfons.cpp -o PSAStringTesterClang -std=c++11

alfons.o: alfons.cpp
	g++ -std=c++11 -c alfons.cpp -fsanitize=leak

run: all
	./PSAStringTester

valgrind: all
	valgrind --leak-check=yes ./PSAStringTester

clean:
	rm -rf *.o *.gch PSAStringTester PSAStringTesterClang