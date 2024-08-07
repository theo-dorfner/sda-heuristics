#include "../include/treeDecomp.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using idList = std::unordered_set<short int>;




// ------------------------------------------------ NODE ------------------------------------------------

// CONSTRUCTOR
Node::Node(short int iD) : id{iD}{};

// RETRIEVAL METHODS

// return ID of node
short int Node::ID() const {return id;}

// return list of neighbours of node
const idList& Node::getNeighbours() const {return neighbours;}

// return list of IDs in the bag
const idList& Node::getBag() const {return bag;}



// NODE BUILDING METHODS

// add vertex ID to bag of node
std::size_t Node::addToBag(short int iD) 
{
    bag.insert(iD);
    return bag.size();
}

// add id as neighbour of this node
void Node::addNeighbour(short int id) {neighbours.insert(id);}








// ------------------------------------------ TREE DECOMPOSITION ------------------------------------------

// RETRIEVAL METHODS

// return current width of TD
int TreeDecomposition::getWidth() const {return width;}

// return current number of nodes
int TreeDecomposition::getNNodes() const {return nNodes;}

// return current number of edges
int TreeDecomposition::getNEdges() const {return nEdges;}

// return list of IDs in the bag with given ID
const idList& TreeDecomposition::getBag(short int nodeID) const {return nodes[nodeID].getBag();}

// return ID of node that has all the given vertices in its bag
short int TreeDecomposition::findNodeIncluding(const idList& vertices) const 
{
    // iterate through and check every node in TD
    // starting from the back to improve performance (attachment of most recent nodes is at the back)
    for(int i{nNodes - 1}; i > -1 ;i--)
    {
        bool next{false};                           // track if we need to check the next node
        const idList &bag = nodes[i].getBag();

        // check if all given vertices are contained in bag
        for(const auto &vertex : vertices)
        {
            if(bag.find(vertex) == bag.end()){
                // vertex was not found in bag -> jump to next node
                next = true;
                break;
            }
        }

        // if loop is finished without jumping out (next = true) we have found a node to return
        if(!next) return nodes[i].ID();
    }
    return -1; // return error code
};



// METHODS FOR TD BUILDING

// create a new node
int TreeDecomposition::addNode()
{
    int id{nNodes++};               // get ID and increment nodes count
    nodes.push_back(Node(id));      // insert newly created node into nodes list
    return id;
}

// create a new edge between two nodes
int TreeDecomposition::addEdge(short int sourceID, short int targetID) 
{
    // check that node IDs are valid
    if (!(sourceID < nNodes && targetID < nNodes) && (sourceID != -1) && (targetID != -1)) return -1;

    // add edge link to both nodes
    nodes[sourceID].addNeighbour(targetID);
    nodes[targetID].addNeighbour(sourceID);

    return nEdges++;     // return edge ID
}

// add a vertex (with given ID) to a node (with given ID)
void TreeDecomposition::addVertexToBag(short int nodeID, short int vertexID)
{
    int bagWidth = nodes[nodeID].addToBag(vertexID) - 1;        // calculate (bag) width of given node
    if(bagWidth > width) width = bagWidth;                      // check if this is larger than the current width (and therefore increases the TD width)
}



// OUTPUTTING

void TreeDecomposition::print() const 
{
    // output nodes
    std::cout << "Nodes [label(ID)]:" << std::endl;
    for(const Node& node : nodes) {
        std::cout << node.ID() << ": ";
        for(const int& iD : node.getBag()) std::cout << iD << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // output edges
    std::cout << "Edges: " << std::endl;
    for(const Node& node : nodes) {
        int nodeID = node.ID();
        for(const int& neighbourID : node.getNeighbours()) {
            if (neighbourID > nodeID) std::cout << nodeID << ", " << neighbourID << std::endl;
        }
    }
}

void TreeDecomposition::output(Graph& graph,std::string filename) const
{
    std::ofstream outputFile (filename, std::ios::out);

    // output nodes
    for(const Node& node : nodes) {
        outputFile << node.ID() << ",,";
        for(const int& iD : node.getBag()) outputFile << graph.getVertex(iD).label << ";";
        outputFile << std::endl;
    }
    
    // output edges
    for(const Node& node : nodes) {
        int nodeID = node.ID();
        for(const int& neighbourID : node.getNeighbours()) {
            if (neighbourID > nodeID) outputFile << nodeID << "," << neighbourID << "," << std::endl;
        }
    }
}