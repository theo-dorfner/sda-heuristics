#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <unordered_set>

#include "include/vertex.hpp"
#include "include/graph.hpp"
#include "include/treeDecomp.hpp"


// define container for holding lists of IDs
using idList = std::unordered_set<short int>;




// RECURSIVE FUNCTION FOR GENERATING A TD BASED ON A GRAPH AND THE GIVEN METHOD
TreeDecomposition decomposition_from_ordering(Graph& graph)
{
    // end condition (only one vertex alive)
    if(graph.activeVertices == 1)
    {
        TreeDecomposition td = TreeDecomposition();                     // create the TD to return
        int newNodeID = td.addNode();                                   // add empty node to TD
        short int lastVertex = graph.next();                            // retrieve last vertex from graph
        graph.eliminate(lastVertex);
        td.addVertexToBag(newNodeID ,lastVertex);                       // add vertex to bag
        return td;
    }


    int eliminatedID = graph.next();                                    // get vertex to eliminate

    // get neighbours of vertex before elimination for later reference
    Vertex& eliminatedVertex = graph.getVertex(eliminatedID);
    const idList& neighbours = eliminatedVertex.getNeighbours();

    graph.eliminate(eliminatedID);


    TreeDecomposition td = decomposition_from_ordering(graph);          // perform decomposition of remaining graph


    // return td;
    short int nodeT = td.findNodeIncluding(neighbours);                 // find node that includes all neighbours
    if(nodeT == -1) std::cout << "Decomposition: Couldn't locate Node that includes all neighbours" << std::endl;        // error catching


    // create new node that contains the eliminated vertex and its neighbours
    int newID = td.addNode();
    td.addVertexToBag(newID, eliminatedID);
    for(auto &elem : neighbours) td.addVertexToBag(newID, elem);

    // add edge between t t'
    td.addEdge(nodeT, newID);


    return td;                                                          // return new td
}




int main(int argc, char *argv[]){

    // ARGUMENT HANDLING

    // check for correct number of arguments
    if (argc != 3) 
    {
        printf("Number of arguments (%i) not suitable, please supply filename and heuristic method \n",argc-1);
        return 0;
    }

    const std::string graphSouceFilename = argv[1];
    const std::string heuristicMethod = argv[2];

    // check for correct spelling of heuristic method
    if(!(heuristicMethod == "mcs" || heuristicMethod == "fill" || heuristicMethod == "degree")){
        std::cout << heuristicMethod << " is not a valid heuristic method. Please choose either mcs, fill or degree" << std::endl;
        return 0;
    }




    // GRAPH FILE READING

    Graph graph;

    std::ifstream graphInput(graphSouceFilename,std::ios::in);
    std::string line;
    std::string e1, e2, e3;

    while (std::getline(graphInput,line)) {
        e1 = line.substr(0,line.find(","));
        line.erase(0,line.find(",")+1);        
        e2 = line.substr(0,line.find(","));
        line.erase(0,line.find(",")+1); 
        e3 = line.substr(0,line.find(","));

        // check if current line is an vertex or edge
        if(e2 != "") {
            // just read an edge
            int sourceID = graph.addVertex(e1);
            int targetID = graph.addVertex(e2);
            graph.addEdge(sourceID, targetID);
        }
    }




    // SET METHOD AND PERFORM METHOD-SPECIFIC ACTIONS

    if(heuristicMethod == "fill") {
        graph.method = FILL;
        graph.countAllFillIn();
    }
    else if(heuristicMethod == "degree") graph.method = DEGREE;
    else if(heuristicMethod == "mcs"){
        graph.method = MCS;
        graph.duplicateNeighbours();
    }
    else{
        std::cout << "problem with matching input method to program method" << std::endl;
        return 0;
    }




    // PERFORM DECOMPOSITION (and elimination) FOR TD
    TreeDecomposition resultingTD = decomposition_from_ordering(graph);


    // output width of generated TD
    std::cout << "width: " << resultingTD.getWidth() << std::endl;


    return 0;
}