#include <string>
#include <map>
#include <cstdio>
#include <climits>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "../include/graph.hpp"
#include "../include/vertex.hpp"

// define container for holding lists of IDs
using idList = std::unordered_set<short int>;



// RETRIEVAL METHODS

// return overall number of nodes
int Graph::getNVertices() const {return nVertices;}

// return overall number of edges
int Graph::getNEdges() const {return nEdges;}

// get ID of a vertex based on its label
short int Graph::getIDfromLabel(std::string label)
{
    if (auto search = labelMapping.find(label); search != labelMapping.end()) {
        return search->second;      // return found label
    }
    else return -1;                 // didn't find a vertex corresponding to this label, return error code
}

// get a vertex object based on its ID
Vertex& Graph::getVertex(short int id) {return vertices[id];}



// GRAPH BUILDING METHODS

// adds new (!) vertex to graph (if vertex already exists, returns vertex ID)
short int Graph::addVertex(std::string label)
{
    // check if vertex already exists
    if (auto search = labelMapping.find(label); search != labelMapping.end()) {
        return search->second;                  // returns found label
    }
    else {
        // label not found, create new vertex
        int id = nVertices++;
        activeVertices++;
        Vertex vertex = Vertex(id, label);

        vertices.push_back(vertex);             // inserte vertex into graph list

        // check if vertex has been correctly added to list
        if (vertices[id] == vertex){
            labelMapping[label] = id;           // create mapping from label to vertex ID
            aliveVertices.insert(id);           // also insert vertex into list of alive vertices
            return id;
        }
        else return -1;                         // if vertex couldn't be added properly, return error code
    }

}

// adds edge between two existing vertices
bool Graph::addEdge(short int sourceID, short int targetID) 
{
    // check that IDs are valid (doesn't check if they are alive however)
    if (!(sourceID < nVertices && targetID < nVertices) && (sourceID != -1) && (targetID != -1)) return -1;

    if(vertices[sourceID].addNeighbour(targetID)){
        if(vertices[targetID].addNeighbour(sourceID)){
            // successfully added edge to both vertices
            return true;    // return success
        }
    }
    return false;      // some error with adding edges
}

// duplicate current neighbours into "original neighbours" (for all vertices)
void Graph::duplicateNeighbours() {
    for(std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end();it++) it->duplicateNeighbours();
};



// METHODS FOR COUNTING FILL-IN EDGES

// counts/updates the fill-in edges
void Graph::countFillIn(Vertex& vertex) 
{
    vertex.requiredFillInEdges = 0;     // reset count of Fill-In Edges
    const idList &baseNeighbours = vertex.getNeighbours();

    // go through all neighbours (primary iterator)
    for(idList::const_iterator primaryNeighbour = baseNeighbours.begin();primaryNeighbour != baseNeighbours.end();primaryNeighbour++){
        // fetch current neighbour (vertex and neighbours of that neighbour)
        Vertex& primaryNVertex = getVertex(*primaryNeighbour);
        const idList &secondaryNeighbours = primaryNVertex.getNeighbours();

        // go through neighbours (of original vertex) until primary neighbour (secondary iterator on  the same list as the primary one)
        for(idList::const_iterator secondaryNeighbour = baseNeighbours.begin(); secondaryNeighbour != primaryNeighbour; secondaryNeighbour++) {
            if(!secondaryNeighbours.contains(*secondaryNeighbour)) vertex.requiredFillInEdges++;            // if primary and secondary neighbours are not connected, increment FillInEdges
        }
    }
};

// wrapper that performs countFillIn(Vertex&) for every vertex
void Graph::countAllFillIn() 
{
    for(auto &vertex : vertices) countFillIn(vertex);
}



// METHODS USED FOR ELIMINATION AND TD GENERATION

// eliminates vertex (with given ID) from graph, builds the neighbour clique (and for fill updates the required fill-in edges)
void Graph::eliminate(short int iD) {
    Vertex& vertex = getVertex(iD);                         // retrieve vertex
    const idList &neighbours = vertex.getNeighbours();      // retrive neighbours
    vertex.eliminated = true;                               // set vertex state to eliminated

    // adapt alive vertices
    aliveVertices.erase(iD);
    activeVertices--;

    // adapt degrees of neighbouring vertices
    if(method == MCS){
        for(const auto &neighbourID : vertex.getOriginalNeighbours()){
            Vertex& neighbour = getVertex(neighbourID);
            if (!(neighbour.eliminated)) neighbour.activeNeighbours++;
        }
    }

    // perform elimination
    // iteration over all neighbours (primary iterator)
    for(idList::const_iterator neighbourIt = neighbours.begin();neighbourIt != neighbours.end();neighbourIt++){
        Vertex& neighbourVertexI = getVertex(*neighbourIt);             // get primary neighbour
        const idList& neighboursI = neighbourVertexI.getNeighbours();   // 
        neighbourVertexI.removeNeighbour(iD);                           // remove vertex as neighbour


        // FILL-IN EDGE ADAPTION 1 - FOR PRIMARY NEIGHBOUR
        // check neighbours of primary iterator/vertex (neighbourIt) if they have original vertex as a neighbour
        // if so, reduce primary neighbours (neighbourIt) requireFillInEdges by one 

        for(const auto& nOfI : neighboursI){
            if(!neighbours.contains(nOfI)) neighbourVertexI.requiredFillInEdges--;
        }

        // CLIQUE BUILDING and (if FILL) FILL-IN EDGE ADAPTION
        // again iteration over all neighbours (secondary iterator) starting from (after) the primary iterator
        for(idList::const_iterator neighbourJIT = std::next(neighbourIt); neighbourJIT != neighbours.end(); neighbourJIT++){
            bool added = addEdge(*neighbourIt,*neighbourJIT);                                       // build clique by adding edge between neighbours
            
            if(method == FILL){
                if(added){
                    // ADAPTION 2 common neighbours have one less edge to fill
                    // FILL-IN EDGE ADAPTION 2 - neighbours between which an edge has been added have one less edge to fill in (for a neighbour common to both primary and secondary itertor)
                    Vertex& neighbourVertexJ = getVertex(*neighbourJIT);
                    const idList& neighboursJ = neighbourVertexJ.getNeighbours();
                    
                    // find common neighbour (between primary and secondary neighbour/iterator)
                    for(const auto& nOfI : neighboursI){
                        // check if that neighbour is also a neighbour of j
                        if(neighboursJ.contains(nOfI)) getVertex(nOfI).requiredFillInEdges--;       // one less edge to fill in for the common neighbour
                        else neighbourVertexI.requiredFillInEdges++;                                // if not a common neighbour, primary iterator/neighbour would have to fill it in upon elimination
                    }

                    // FILL-IN EDGE ADAPTION 3 - for both neighbours, there is one more edge to build edges to
                    // (the case for the primary neighbour (neighbourVertexI) is for performance reasons covered in the above "else" clause)
                    for(const auto& nOfJ : neighboursJ) if(!neighboursI.contains(nOfJ))neighbourVertexJ.requiredFillInEdges++;
                }
            }
        }
    }
}

// returns next verted (ID) to be eliminated based on given heuristics method
short int Graph::next() {
    if(method == FILL){
        int minValue{INT_MAX}, winner{-1}, active;                  // setting high minValue so that at least one (optimally every) vertex is smaller (fill method requires minimality)
        for(idList::const_iterator vertexID = aliveVertices.begin(); vertexID != aliveVertices.end();vertexID++){
            active = getVertex(*vertexID).requiredFillInEdges;      // get relevant measure for given heuristic

            // check if value is better than the currently selected one
            if(active < minValue){
                minValue = active;
                winner = *vertexID;
            }
        }
        return winner;
    }
    else if(method == DEGREE){
        int minValue{nVertices}, winner{-1}, active;                // setting high minValue so that at least one (optimally every) vertex is smaller (degree method requires minimality)
        for(idList::const_iterator vertexID = aliveVertices.begin(); vertexID != aliveVertices.end();vertexID++){
            active = getVertex(*vertexID).activeDegree;             // get relevant measure for given heuristic
 
            // check if value is better than the currently selected one
            if(active < minValue){
                minValue = active;
                winner = *vertexID;
            }
        }
        return winner;
    }
    else if(method == MCS){
        int maxValue{-1}, winner{-1}, active;                       // setting low maxValue so that every vertex is larger (mcs requires maximality)
        for(idList::const_iterator vertexID = aliveVertices.begin(); vertexID != aliveVertices.end();vertexID++){
            active = getVertex(*vertexID).activeNeighbours;         // get relevant measure for given heuristic
        
            // check if value is better than the currently selected one
            if(active > maxValue){
                maxValue = active;
                winner = *vertexID;
            }
        }
        return winner;
    }
    else{
        // error message and code return
        std::cout << "next(): there is a problem with the selected method: " << method << std::endl;
        return -1;
    };
};



// OUTPUTTING

// prints all (connected) vertices to console
void Graph::print() const 
{
    // print list of vertices
    std::cout << "Vertices [format: label (ID, fillInEdges, activeDegree)]:" << std::endl;
    for(const Vertex& vertex : vertices) {
        std::cout << vertex.label << "( " << vertex.ID() << ", " << vertex.eliminated << ", " << vertex.requiredFillInEdges << ", " << vertex.activeDegree << "): " ;
        for(const int& elem : vertex.getNeighbours()) std::cout << elem << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // print sorted list of edges
    std::cout << "Edges: " << std::endl;
    for(const Vertex& vertex : vertices) {
        int vertexID = vertex.ID();
        for(const int& neighbourID : vertex.getNeighbours()) {
            if (neighbourID > vertexID) std::cout << vertex.ID() << ", " << vertices[neighbourID].ID() << std::endl;
        }
    }
}