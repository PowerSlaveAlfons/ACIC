all: PSAString

PSAString: main.o PSAString.o
	g++ -std=c++17 *.o -o PSAStringTester -fsanitize=leak

main.o: main.cpp
	g++ -std=c++17 -c main.cpp -fsanitize=leak
	
PSAString.o: PSAString.h PSAString.cpp
	g++ -std=c++17 -c PSAString.cpp -fsanitize=leak

run: all
	./PSAStringTester

clean:
	rm -rf *.o *.gch PSAStringTester