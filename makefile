main:
	g++ -Wall src/main.cpp -o build/main.out

run: main
	./build/main.out tests/test3.txt
