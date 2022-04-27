.PHONY: all clean

all: buildsa querysa

clean:
	rm buildsa querysa

buildsa: src/buildsa.cpp
	g++ -g -O3 -std=c++20 -IFastaReader/include -Iinclude -Icereal/include -LFastaReader/build/lib -Llib -o buildsa -Wl,-R -Wl,/home/jhdavis/repos/cmsc858d/hw2/FastaReader/build/lib/ src/buildsa.cpp -lFastaReader -lsdsl

querysa: src/querysa.cpp
	g++ -g -O3 -std=c++20 -IFastaReader/include -Iinclude -Icereal/include -LFastaReader/build/lib -Llib -o querysa -Wl,-R -Wl,/home/jhdavis/repos/cmsc858d/hw2/FastaReader/build/lib/ src/querysa.cpp -lFastaReader -lsdsl
