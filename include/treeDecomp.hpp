#pragma once
#include <vector>
#include <string>

#include "vertex.hpp"

using idList = std::vector<short int>;

class Node 
{
    short int id;
    idList neighbours; // could also be pointers
    idList bag;
    int degree{0};

public:
    Node(short int);
    short int ID() const;

    void addNeighbour(short int id);

    const idList& getNeighbours() const;

    const idList& getBag() const;

    std::size_t addToBag(short int);

};


class TreeDecomposition
{
    int width{-1}, nNodes{0}, nEdges{0};
    std::vector<Node> nodes;

public:
    TreeDecomposition(){};
    int getWidth() const;
    int getNNodes() const; // return nNodes
    int getNEdges() const; // return nNodes

    int addNode();
    int addEdge(short int, short int);

    void addVertexToBag(short int, short int);

    const idList& getBag(short int) const;

    void print() const;

    short int findNodeIncluding(const std::vector<short int>&) const;
};