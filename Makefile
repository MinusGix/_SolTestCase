.PHONY: build

build:
	clang++ -std=c++17 -g -llua ./main.cpp -o program && ./program