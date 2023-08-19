CCC = g++ -std=c++11
FLAGS = -c -O3 -m64 -g

loggi: $(CVRPSEP) brkga.o decoder.o distance.o local_search.o read.o
	g++ brkga.o decoder.o distance.o local_search.o read.o -lm -o LOGGI

brkga.o: BRKGA_QL.cpp
	$(CCC) $(FLAGS) BRKGA_QL.cpp -lm -o brkga.o

decoder.o: Decoder.cpp
	$(CCC) $(FLAGS) Decoder.cpp -lm -o decoder.o

distance.o: Distance.cpp
	$(CCC) $(FLAGS) Distance.cpp -lm -o distance.o

local_search.o: LocalSearch.cpp
	$(CCC) $(FLAGS) LocalSearch.cpp -lm -o local_search.o

read.o: Read.cpp
	$(CCC) $(FLAGS) Read.cpp -lm -o read.o

clean: 
	rm -rf *.o *~

