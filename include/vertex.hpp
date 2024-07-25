#pragma once

#include <string>
#include <unordered_set>
#include <vector>


// define container for holding lists of IDs
using idList = std::unordered_set<short int>;


class Vertex
{
    short int id;                   // ID of vertex
    idList neighbours;              // list of IDs of the neighbours
    idList originalNeighbours;      // maintain copy of original neighbours before start of elimination
    short int degree{0};    

public:
    int activeDegree{0}, requiredFillInEdges{0}, activeNeighbours{0};   // values specific to the methods
    std::string label;              // external label for this vertex (e.g. from input file)
    bool eliminated{false};         // check if vertex has been eliminated

    // CONSTRUCTORS
    Vertex(short int);
    Vertex(short int, std::string);

    // RETRIEVAL METHODS
    int ID() const;
    int dgr() const;
    const idList& getNeighbours () const;
    const idList& getOriginalNeighbours () const; // retrieves original neighbours


    // add neighbour to list of neighbours
    bool addNeighbour(short int id);

    // Comparison of vertices
    bool operator==(const Vertex&) const;

    // remove neighbour from list of neighbours
    void removeNeighbour(short int);

    // cleares and populates originalNeighbours from current list of neighbours
    void duplicateNeighbours();

};