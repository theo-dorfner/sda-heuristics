#include "../include/graph.hpp"
#include "../include/vertex.hpp"

#include <string>
#include <map>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <unordered_set>

using idList = std::unordered_set<short int>;



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

bool Graph::addEdge(short int sourceID, short int targetID) 
{
    if (!(sourceID < nVertices && targetID < nVertices) && (sourceID != -1) && (targetID != -1)) return -1;

    if(vertices[sourceID].addNeighbour(targetID)){
        vertices[targetID].addNeighbour(sourceID);
        return true; // return edge ID
    }
    else return false;
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


void Graph::countAllFillIn() 
{
    int i{0};
    for(auto &vertex : vertices){
        countFillIn(vertex);
        // std::cout << "updating " << i++ << std::endl;
    }
}

void Graph::countFillIn(Vertex& vertex) 
{
    vertex.requiredFillInEdges = 0;
    const idList &baseNeighbours = vertex.getNeighbours();
    for(idList::const_iterator primaryNeighbour = baseNeighbours.begin();primaryNeighbour != baseNeighbours.end();primaryNeighbour++){
        Vertex& primaryNVertex = getVertex(*primaryNeighbour);
        // if(primaryNVertex.eliminated) continue;

        const idList &secondaryNeighbours = primaryNVertex.getNeighbours();
        for(idList::const_iterator secondaryNeighbour = baseNeighbours.begin(); secondaryNeighbour != primaryNeighbour; secondaryNeighbour++) {
            // if(getVertex(*secondaryNeighbour).eliminated) continue;
            bool found{false};

            if(secondaryNeighbours.find(*secondaryNeighbour) == secondaryNeighbours.end()) vertex.requiredFillInEdges++;
        }


    }
    // for(auto &n0 : neighbours){
    //     const idList &secondNeighbours = neighbour.getNeighbours();
    //     for(auto &n1 : neighbours){
    //         if(getVertex(n1).eliminated) continue;
    //         if(n1 >= n0) break;
    //         bool found{false};
    //         for(auto &n2 : secondNeighbours){
    //             if(n2 > n1) break;
    //             if(n1 == n2){found = true; break;}
    //         }
    //         if(!found) vertex.requiredFillInEdges++;
    //     }
    // }
};

void Graph::eliminate(short int iD) {
    // std::cout << "elimination" << std::endl;
    // std::cout << " attempting to eliminate " << iD << " -" << getVertex(iD).label << std::endl;
    Vertex& vertex = getVertex(iD);
    vertex.eliminated = true;
    activeVertices--;
    const idList &neighbours = vertex.getNeighbours();
    // std::cout << "loop:" << std::endl;
    for(idList::const_iterator neighbourIt = neighbours.begin();neighbourIt != neighbours.end();neighbourIt++){
    // for(int i{0}; i < neighbours.size();i++){
        // std::cout << "." << std::endl;
        // std::cout << "  from " << neighbour << " -" << getVertex(neighbour).label << std::endl;
        // auto neighbour = neighbours[i];
        Vertex& neighbourVertexI = getVertex(*neighbourIt);
        // if(!) std::cout << "couldn't delete" << std::endl;
        neighbourVertexI.removeNeighbour(iD);

        // generate clique
        // std::cout << "e" << std::endl;

        for(idList::const_iterator neighbourJIT = std::next(neighbourIt); neighbourJIT != neighbours.end(); neighbourJIT++){
            bool added = addEdge(*neighbourIt,*neighbourJIT);
            
            if(fillIn){
                if(added){
                    const idList& neighboursI = neighbourVertexI.getNeighbours();

                    Vertex& neighbourVertexJ = getVertex(*neighbourJIT);
                    const idList& neighboursJ = neighbourVertexJ.getNeighbours();
                    // for every neighbour if I
                    for(const auto& nOfI : neighboursI){
                        // check if that neighbour is also a neighbour of j
                        if(neighboursJ.find(nOfI) != neighboursJ.end()){
                            // getVertex(nOfI).requiredFillInEdges--;
                        }
                    }
                    // if so, reduce neighbours fill in edges by one
                }
            }
        }
        // for(int j{i+1}; j < neighbours.size(); j++) addEdge(neighbour,neighbours[j]);
        // std::cout << "done" << std::endl;
        
    }
    // vertex.clearNeighbours();
    // std::cout << " done" << std::endl;

}