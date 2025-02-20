CXX=g++-11

CXXFLAGS = -std=c++20 -O3 -pedantic -march=native -ffast-math 

.DEFAULT_GOAL := main

main: heuristics.cpp libraries
	@$(CXX) -o build/heuristics heuristics.cpp build/vertex.o build/graph.o build/treeDecomp.o $(CXXFLAGS)


libraries: vertex graph treeDecomp

treeDecomp: include/treeDecomp.hpp lib/treeDecomp.cpp graph
	@$(CXX) -c -o build/treeDecomp.o lib/treeDecomp.cpp $(CXXFLAGS)

graph: include/graph.hpp lib/graph.cpp
	@$(CXX) -c -o build/graph.o lib/graph.cpp $(CXXFLAGS)

vertex: include/vertex.hpp lib/vertex.cpp
	@$(CXX) -c -o build/vertex.o lib/vertex.cpp $(CXXFLAGS)

clean:
	@rm build/*.o