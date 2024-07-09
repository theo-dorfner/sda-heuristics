#include "../include/vertex.hpp"

#include <string>
#include <iostream>
#include <algorithm>

Vertex::Vertex(short int iD) : id{iD}, label{std::to_string(iD)} {}
Vertex::Vertex(short int iD, std::string _label) : id{iD}, label{_label} {}

int Vertex::ID() const {return id;}

int Vertex::dgr() const {return degree;}

bool Vertex::addNeighbour(short int iD) 
{
    auto state = neighbours.insert(iD);
    if(state.second){
        degree++;
        activeDegree++;
        // std::cout << iD << ", d: " << degree << ", aD: " << activeDegree << std::endl;
        return true;
    }
    else return false;
}

const idList& Vertex::getNeighbours() const {return neighbours;}

bool Vertex::operator==(const Vertex& other) const 
{
    return (id == other.id) && (label == other.label) && (neighbours == other.neighbours);
}


void Vertex::removeNeighbour(short int iD) 
{
    neighbours.erase(iD);
    degree--;
    activeDegree--;
};

void Vertex::clearNeighbours(){neighbours.clear();}