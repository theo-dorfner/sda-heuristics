#pragma once

#include <vector>
#include <map>
#include <string>

#include "vertex.hpp"


class Graph
{
    short int nVertices{0};
    int nEdges{0};
    std::vector<Vertex> vertices;
    std::map<std::string, short int> labelMapping;

public:
    int activeVertices{0};
    bool fillIn{false};

    int getNVertices() const; // return nNodes
    int getNEdges() const; // return nEdges


    void resetEliminations(); // reset elimination states of all nodes

    short int addVertex(std::string); //can also act as check if vertex already exists

    bool addEdge(short int, short int);

    short int getIDfromLabel(std::string);

    Vertex& getVertex(short int);

    void print() const;

    void countAllFillIn();

    void countFillIn(Vertex&);

    void eliminate(short int);

};