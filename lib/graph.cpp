#include "../include/graph.hpp"
#include "../include/vertex.hpp"

#include <string>
#include <map>
#include <cstdio>
#include <iostream>
#include <algorithm>


int Graph::getNVertices() const {return nVertices;}

int Graph::getNEdges() const {return nEdges;}

void Graph::resetEliminations()
{
    for(auto& vertex : vertices) vertex.eliminated = false;
}

short int Graph::addVertex(std::string label)
{
    // check if vertex already exists
    if (auto search = labelMapping.find(label); search != labelMapping.end()) {
        // found label
        return search->second;
    }
    else {
        // label not found, create new vertex
        int id = nVertices++;
        activeVertices++;
        Vertex vertex = Vertex(id, label);

        vertices.push_back(vertex); 
        if (vertices[id] == vertex){
            labelMapping[label] = id;
            return id;
        }
        else return -1;
        
    }

}

int Graph::addEdge(short int sourceID, short int targetID) 
{
    if (!(sourceID < nVertices && targetID < nVertices) && (sourceID != -1) && (targetID != -1)) return -1;
    vertices[sourceID].addNeighbour(targetID);
    vertices[targetID].addNeighbour(sourceID);
    return nEdges++; // return edge ID
}

short int Graph::getIDfromLabel(std::string label)
{
    if (auto search = labelMapping.find(label); search != labelMapping.end()) {
        // found label
        return search->second;
    }
    else return -1;
}

Vertex& Graph::getVertex(short int id) {return vertices[id];}

// only prints connected vertices
void Graph::print() const 
{
    std::cout << "Vertices [label (ID, fillInEdges, degree)]:" << std::endl;
    for(const Vertex& vertex : vertices) {std::cout << vertex.label << "( " << vertex.ID() << ", " << vertex.eliminated << ", " << vertex.requiredFillInEdges << ", " << vertex.activeDegree << "): " ;
    for(const int& elem : vertex.getNeighbours()) std::cout << elem << ", ";
    std::cout << std::endl;}
    std::cout << std::endl;

    std::cout << "Edges: " << std::endl;
    for(const Vertex& vertex : vertices) {
        int vertexID = vertex.ID();
        for(const int& neighbourID : vertex.getNeighbours()) {
            if (neighbourID > vertexID) std::cout << vertex.ID() << ", " << vertices[neighbourID].ID() << std::endl;
        }
    }
}


void Graph::countFillIn() 
{
    for(auto &vertex : vertices)updateFillIn(vertex);
}

void Graph::updateFillIn(Vertex& vertex) 
{
    const std::vector<short int> &neighbours = vertex.getNeighbours();
    for(auto &n0 : neighbours){
        Vertex& neighbour = getVertex(n0);
        if(neighbour.eliminated) continue;
        const std::vector<short int> &secondNeighbours = neighbour.getNeighbours();
        for(auto &n1 : neighbours){
            if(getVertex(n1).eliminated) continue;
            if(n1 >= n0) break;
            bool found{false};
            for(auto &n2 : secondNeighbours){
                if(n2 > n1) break;
                if(n1 == n2){found = true; break;}
            }
            if(!found) vertex.requiredFillInEdges++;
        }
    }
};

void Graph::eliminate(short int iD) {
    // std::cout << " attempting to eliminate " << iD << " -" << getVertex(iD).label << std::endl;
    getVertex(iD).eliminated = true;
    activeVertices--;
    const std::vector<short int> &neighbours = getVertex(iD).getNeighbours();
    for(int i{0}; i < neighbours.size();i++){
        // std::cout << "  from " << neighbour << " -" << getVertex(neighbour).label << std::endl;
        auto neighbour = neighbours[i];
        Vertex& neighbourVertex = getVertex(neighbour);
        if(!neighbourVertex.removeNeighbour(iD)) std::cout << "couldn't delete" << std::endl;

        // generate clique
        for(int j{i+1}; j < neighbours.size(); j++) addEdge(neighbour,neighbours[j]);

    }

}