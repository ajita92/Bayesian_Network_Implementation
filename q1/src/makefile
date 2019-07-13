all: generate query

generate: Q1a.cpp
	g++ -std=c++11 Q1a.cpp -o generateBN.out

query: Q1bextra.cpp
	g++ -std=c++11 Q1bextra.cpp -o queryBN.out
	
clean:
	rm generateBN.out queryBN.out *.o 
