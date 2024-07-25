#pragma once

#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "vertex.hpp"


// define container for holding lists of IDs
using idList = std::unordered_set<short int>;

// heuristics method
enum heuristic {
    FILL,
    DEGREE,
    MCS
};



// container for holding graph
class Graph
{
    short int nVertices{0};                         // track number of vertices
    int nEdges{0};                                  // track number of edges
    std::vector<Vertex> vertices;                   // list/vector of all vertices in graph
    std::map<std::string, short int> labelMapping;  // map for finding ID based on label (string)
    idList aliveVertices;                           // list of vertices that aren't eliminated yet

public:
    enum heuristic method;
    int activeVertices{0};

    // RETRIEVAL METHODS
    int getNVertices() const;                       // return overall number of nodes
    int getNEdges() const;                          // return overall number of edges
    short int getIDfromLabel(std::string);          // get ID of a vertex based on its label
    Vertex& getVertex(short int);                   // get a vertex object based on its ID


    // GRAPH BUILDING METHODS
    short int addVertex(std::string);               // adds new (!) vertex to graph
    bool addEdge(short int, short int);             // adds edge between two existing vertices
    void duplicateNeighbours();                     // duplicate current neighbours into "original neighbours" (for all vertices)


    // METHODS FOR COUNTING FILL-IN EDGES
    void countFillIn(Vertex&);                      // counts/updates the fill-in edges
    void countAllFillIn();                          // wrapper that performs countFillIn(Vertex&) for every vertex


    // METHODS USED FOR ELIMINATION AND TD GENERATION
    void eliminate(short int);                      // eliminates vertex (with given ID) from graph, builds the neighbour clique (and for fill updates the required fill-in edges)
    short int next();                               // returns next verted (ID) to be eliminated based on given heuristics method


    // OUTPUTTING
    void print() const;
};