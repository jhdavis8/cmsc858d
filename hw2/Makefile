.PHONY: all clean

all: buildsa

clean:
	rm buildsa querysa

buildsa: src/buildsa.cpp
	g++ -O3 -std=c++20 -IFastaReader/include -Iinclude -Icereal/include -LFastaReader/build/lib -Llib -o buildsa src/buildsa.cpp -lFastaReader -lsdsl
