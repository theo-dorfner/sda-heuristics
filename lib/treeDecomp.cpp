#include "../include/treeDecomp.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>

using idList = std::unordered_set<short int>;

// NODE
Node::Node(short int iD) : id{iD}{};

short int Node::ID() const {return id;}

void Node::addNeighbour(short int id) {neighbours.insert(id);}

const idList& Node::getNeighbours() const {return neighbours;}

const idList& Node::getBag() const {return bag;}

std::size_t Node::addToBag(short int iD) 
{
    bag.insert(iD);
    return bag.size();
}




// TREE DECOMPOSITION
int TreeDecomposition::getWidth() const {return width;}
int TreeDecomposition::getNNodes() const {return nNodes;}
int TreeDecomposition::getNEdges() const {return nEdges;}

int TreeDecomposition::addNode()
{
    int id{nNodes++};
    nodes.push_back(Node(id));
    return id;
}

int TreeDecomposition::addEdge(short int sourceID, short int targetID) 
{
    if (!(sourceID < nNodes && targetID < nNodes) && (sourceID != -1) && (targetID != -1)) return -1;
    nodes[sourceID].addNeighbour(targetID);
    nodes[targetID].addNeighbour(sourceID);
    return nEdges++; // return edge ID
}


void TreeDecomposition::addVertexToBag(short int nodeID, short int vertexID)
{
    int bagWidth = nodes[nodeID].addToBag(vertexID) - 1;
    if(bagWidth > width) width = bagWidth;
}

const idList& TreeDecomposition::getBag(short int nodeID) const {return nodes[nodeID].getBag();}

void TreeDecomposition::print() const 
{
    std::cout << "Nodes [label(ID)]:" << std::endl;
    for(const Node& node : nodes) {
        std::cout << node.ID() << ": ";
        for(const int& iD : node.getBag()) std::cout << iD << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Edges: " << std::endl;
    for(const Node& node : nodes) {
        int nodeID = node.ID();
        for(const int& neighbourID : node.getNeighbours()) {
            if (neighbourID > nodeID) std::cout << nodeID << ", " << neighbourID << std::endl;
        }
    }
}

short int TreeDecomposition::findNodeIncluding(const idList& vertices) const 
{
    // std::cout << "trying to find: ";
    // for (auto &elem : vertices) std::cout << elem << " ";
    // std::cout << std::endl;
    for(int i{nNodes - 1}; i > -1 ;i--)
    {
        bool next{false};
        const idList &bag = nodes[i].getBag();
        // std::cout << "  node " << i << ": ";
        // for (auto &elem : bag) std::cout << elem << " ";
        // std::cout << std::endl;
        for(const auto &vertex : vertices)
        {
            if(bag.find(vertex) == bag.end()){
                next = true;
                break;
            }
        }
        if(!next) return nodes[i].ID();
    }
    return -1;
};

