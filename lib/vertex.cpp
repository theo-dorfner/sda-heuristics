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
    std::sort(neighbours.begin(),neighbours.end());
    for(std::vector<short int>::iterator it = neighbours.begin();it != neighbours.end();){
        if(*it < iD) it++;
        else if (*it == iD) return false;
        else if (*it > iD) {
            neighbours.insert(it,iD);
            degree++;
            activeDegree++;
            return true;
        }
    }
    neighbours.insert(neighbours.end(),iD);
    degree++;
    activeDegree++;
    return true;
}

const idList& Vertex::getNeighbours() const {return neighbours;}

bool Vertex::operator==(const Vertex& other) const 
{
    return (id == other.id) && (label == other.label) && (neighbours == other.neighbours);
}


bool Vertex::removeNeighbour(short int iD) 
{
    for (std::vector<short int>::iterator it = neighbours.begin(); it != neighbours.end();)
    // for(int i{0};i < neighbours.size();i++)
    {
        // std::cout << *it << std::endl;
        // std::cout << "      N:" << neighbours[i] << std::endl;
        if(*it == iD){
        // if(neighbours[i] == iD){
            neighbours.erase(it);
            return true;
        }
        else
            ++it;
    }
    return false;
};
