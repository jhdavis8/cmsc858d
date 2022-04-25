.PHONY: all clean

all: buildsa querysa

clean:
	rm buildsa querysa

buildsa: src/buildsa.cpp
	g++ -O3 -std=c++20 -IFastaReader/include -Iinclude -Icereal/include -LFastaReader/build/lib -Llib -o buildsa src/buildsa.cpp -lFastaReader -lsdsl

querysa: src/querysa.cpp
	g++ -O3 -std=c++20 -IFastaReader/include -Iinclude -Icereal/include -LFastaReader/build/lib -Llib -o querysa src/querysa.cpp -lFastaReader -lsdsl
