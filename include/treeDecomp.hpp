#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "vertex.hpp"


// define container for holding lists of IDs
using idList = std::unordered_set<short int>;


// container for holding single nodes of a tree decomposition
class Node 
{
    short int id;                           // ID of container
    idList neighbours;                      // list of IDs of the neighbours
    idList bag;                             // list IDs of vertices in the node's bag
    // short int degree{0};                    

public:

    // CONSTRUCTOR
    Node(short int);

    // RETRIEVAL METHODS
    short int ID() const;                   // return ID of node
    const idList& getNeighbours() const;    // return list of neighbours of node
    const idList& getBag() const;           // return list of IDs in the bag

    // NODE BUILDING METHODS
    std::size_t addToBag(short int);        // add vertex ID to bag of node and return size of bag after insertion
    void addNeighbour(short int id);        // add id as neighbour of this node
};




// container for holding a tree decomposition
class TreeDecomposition
{
    int width{-1};                                          // keep track of width of elimination ordering (max width of any bag)
    int nNodes{0}, nEdges{0};                               // number of nodes and edges                 
    std::vector<Node> nodes;                                // a list of nodes in the TD

public:

    // RETRIEVAL METHODS
    int getWidth() const;                                   // return current width of TD
    int getNNodes() const;                                  // return current number of nodes
    int getNEdges() const;                                  // return current number of edges
    const idList& getBag(short int) const;                  // return list of IDs in the bag with given ID
    short int findNodeIncluding(const idList&) const;       // return ID of node that has all the given vertices in its bag

    // METHODS FOR TD BUILDING
    int addNode();                                          // create a new node
    int addEdge(short int, short int);                      // create a new edge between two nodes
    void addVertexToBag(short int, short int);              // add a vertex (with given ID) to a node (with given ID)

    // OUTPUTTING
    void print() const;
};