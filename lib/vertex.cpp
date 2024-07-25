#include <algorithm>
#include <iostream>
#include <string>

#include "../include/vertex.hpp"


// CONSTRUCTORS
Vertex::Vertex(short int iD) : id{iD}, label{std::to_string(iD)} {}

Vertex::Vertex(short int iD, std::string _label) : id{iD}, label{_label} {}



// RETRIEVAL METHODS
int Vertex::ID() const {return id;}

int Vertex::dgr() const {return degree;}

const idList& Vertex::getNeighbours() const {return neighbours;}

const idList& Vertex::getOriginalNeighbours() const {return originalNeighbours;}



// add neighbour to list of neighbours
bool Vertex::addNeighbour(short int iD) 
{
    auto state = neighbours.insert(iD);

    // insert successful (because iD wasn't a neighbour already)
    if(state.second){
        degree++;
        activeDegree++;
        return true;
    }
    else return false;
}



// Comparison of vertices
bool Vertex::operator==(const Vertex& other) const 
{
    return (id == other.id) && (label == other.label) && (neighbours == other.neighbours);
}



// remove neighbour from list of neighbours
void Vertex::removeNeighbour(short int iD) 
{
    neighbours.erase(iD);
    degree--;
    activeDegree--;
};



// cleares and populates originalNeighbours from current list of neighbours
void Vertex::duplicateNeighbours() {
    originalNeighbours.clear(); // 
    for(idList::const_iterator id = neighbours.begin(); id != neighbours.end();id++) originalNeighbours.insert(*id);
};
