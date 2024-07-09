#pragma once
#include <vector>
#include <string>
#include <unordered_set>

using idList = std::unordered_set<short int>;

class Vertex
{
    short int id;
    idList neighbours; // could also be pointers
    short int degree{0};

public:
    int activeDegree{0}, requiredFillInEdges{0},activeNeighbours{0};
    std::string label;
    bool eliminated{false};

    Vertex(short int);
    Vertex(short int, std::string);

    int ID() const;

    int dgr() const;

    bool addNeighbour(short int id);

    const idList& getNeighbours () const;

    bool operator==(const Vertex&) const;

    void removeNeighbour(short int);

    void clearNeighbours();

};