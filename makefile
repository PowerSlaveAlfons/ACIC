all: PSAString

PSAString: main.o PSAString.o
	g++ -std=c++11 *.o -o PSAStringTester

main.o: main.cpp
	g++ -std=c++11 -c main.cpp
	
PSAString.o: PSAString.h PSAString.cpp
	g++ -std=c++11 -c PSAString.cpp

run: all
	./PSAStringTester

clean:
	rm -rf *.o *.gch PSAStringTester