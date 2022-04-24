.PHONY: all clean

all: buildsa

clean:
	rm buildsa querysa

buildsa: src/buildsa.cpp
	g++ -g -std=c++20 -o buildsa src/buildsa.cpp
