#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>

#include "include/vertex.hpp"
#include "include/graph.hpp"
#include "include/treeDecomp.hpp"

TreeDecomposition decomposition_from_ordering(Graph graph, std::vector<int>& eliminationOrdering)
{
    if(eliminationOrdering.size() == 1)
    {
        // std::cout << "reached bottom: " << graph.getVertex(eliminationOrdering[0]).ID() << std::endl;
        TreeDecomposition td = TreeDecomposition();
        int newNodeID = td.addNode();
        td.addVertexToBag(newNodeID ,eliminationOrdering[0]);
        return td;
    }

    int eliminatedID = eliminationOrdering[0];
    eliminationOrdering.erase(eliminationOrdering.begin());

    // find node s.t. neighbours included in bag
    Vertex& eliminatedVertex = graph.getVertex(eliminatedID);
    const std::vector<short int> &neighbours = eliminatedVertex.getNeighbours();
    
    graph.eliminate(eliminatedID);
    // std::cout << "GRAPH AFTER ELIMINATION of " << eliminatedID << std::endl;
    // graph.print();
    TreeDecomposition td = decomposition_from_ordering(graph, eliminationOrdering);


    // std::cout << "&&reached " << eliminationOrdering.size() << std::endl;



    // std::cout << "eliminated: " << eliminatedID << std::endl << "neighbours: ";
    // for (auto &elem : neighbours) std::cout << elem << " ";
    // std::cout << std::endl;


    short int nodeT = td.findNodeIncluding(neighbours);
    if(nodeT == -1) std::cout << "serious problem" << std::endl;


    // create new node with neighbours + v in bag
    int newID = td.addNode();
    td.addVertexToBag(newID, eliminatedID);
    for(auto &elem : neighbours) td.addVertexToBag(newID, elem);

    // add edge between t t'
    td.addEdge(nodeT, newID);

    // return new td
    return td;

}

int main(int argc, char *argv[]){

    // read arguments
    // mcs, fill, degree
    if (argc != 3) 
    {
        printf("Number of arguments (%i) not suitable, please supply filename and heuristic method \n",argc-1);
        return 0;
    }
    const std::string graphSouceFilename = argv[1];
    const std::string heuristicMethod = argv[2];
    if(!(heuristicMethod == "mcs" || heuristicMethod == "fill" || heuristicMethod == "degree")){
        std::cout << heuristicMethod << " is not a valid heuristic method. Please choose either mcs, fill or degree" << std::endl;
        return 0;
    }

    // read in and transform graph from csv format
    Graph graph;
    std::string line;
    std::ifstream graphInput(graphSouceFilename,std::ios::in);
    std::string e1, e2, e3;
    int ctr{0};
    while (std::getline(graphInput,line)) {
        e1 = line.substr(0,line.find(","));
        line.erase(0,line.find(",")+1);        
        e2 = line.substr(0,line.find(","));
        line.erase(0,line.find(",")+1); 
        e3 = line.substr(0,line.find(","));

        if(e2 != "") {
            // just read an edge
            int sourceID = graph.addVertex(e1);
            int targetID = graph.addVertex(e2);
            graph.addEdge(sourceID, targetID);
        }
    }

    // printf("BEFORE ORDERING: \n");
    // graph.print();

    // generate elimination ordering (heuristics)
    std::vector<int> eliminationOrdering;
    int eliminationSorts{0};
    if (heuristicMethod == "degree") 
    {
        std::vector<short int> startingVertices;
        for(short int i{0}; i < graph.getNVertices();i++) startingVertices.push_back(i);

        // perform initial sort
        std::sort(startingVertices.begin(),startingVertices.end(),[&graph](int first, int second){return (graph.getVertex(first).activeDegree < graph.getVertex(second).activeDegree);});

        // for (auto &elem : startingVertices) std::cout << elem << " " << graph.getVertex(elem).activeDegree << std::endl;



        // OUTPUT
        // for (auto &elem : startingVertices) std::cout << "(" << graph.getVertex(elem).label << "," << graph.getVertex(elem).activeDegree << ") ";
        // std::cout << std::endl;


        // start elimination
        for(int i{0}; i < graph.getNVertices();i++) 
        {
            int eliminatedID = startingVertices[0];
            startingVertices.erase(startingVertices.begin());
            eliminationOrdering.push_back(eliminatedID);

            Vertex eliminatedVertex = graph.getVertex(eliminatedID);
            eliminatedVertex.eliminated = true;

            const std::vector<short int> &neighbours = eliminatedVertex.getNeighbours();

            for(const auto &neighbourID : neighbours){
                Vertex& neighbour = graph.getVertex(neighbourID);
                if (!(neighbour.eliminated)) neighbour.activeDegree--;
            }


            // perform checkup sort
            std::sort(startingVertices.begin(),startingVertices.end(),[&graph,&eliminationSorts](int first, int second){bool stays = (graph.getVertex(first).activeDegree < graph.getVertex(second).activeDegree); if (!stays) eliminationSorts++; return stays;});

            // Generating OUTPUT
            // std::cout << graph.getVertex(eliminatedID).label ;
            // for (auto &elem : startingVertices) std::cout << "(" << graph.getVertex(elem).ID() << "," << graph.getVertex(elem).activeDegree << ") ";
            // std::cout << eliminationSorts << std::endl;
        }
        
    }


    else if (heuristicMethod == "fill") {
        // fill in values of fillIn edges
        graph.countFillIn();

        std::vector<short int> startingVertices;
        for(short int i{0}; i < graph.getNVertices();i++) startingVertices.push_back(i);

        // perform initial sort
        std::sort(startingVertices.begin(),startingVertices.end(),[&graph](int first, int second){return (graph.getVertex(first).requiredFillInEdges < graph.getVertex(second).requiredFillInEdges);});


        // OUTPUT
        // for (auto &elem : startingVertices) std::cout << "(" << graph.getVertex(elem).label << "," << graph.getVertex(elem).requiredFillInEdges << ") ";
        // std::cout << std::endl;


        // start elimination
        for(int i{0}; i < graph.getNVertices();i++) 
        {
            int eliminatedID = startingVertices[0];
            startingVertices.erase(startingVertices.begin());
            eliminationOrdering.push_back(eliminatedID);

            Vertex eliminatedVertex = graph.getVertex(eliminatedID);
            eliminatedVertex.eliminated = true;

            const std::vector<short int> &neighbours = eliminatedVertex.getNeighbours();

            for(const auto &neighbourID : neighbours){
                Vertex& neighbour = graph.getVertex(neighbourID);
                if (!(neighbour.eliminated)) graph.updateFillIn(neighbour);
            }


            // perform checkup sort
            std::sort(startingVertices.begin(),startingVertices.end(),[&graph,&eliminationSorts](int first, int second){bool stays = (graph.getVertex(first).activeDegree < graph.getVertex(second).activeDegree); if (!stays) eliminationSorts++; return stays;});

            // Generating OUTPUT
            // std::cout << graph.getVertex(eliminatedID).label ;
            // for (auto &elem : startingVertices) std::cout << "(" << graph.getVertex(elem).label << "," << graph.getVertex(elem).requiredFillInEdges << ") ";
            // std::cout << eliminationSorts << std::endl;
        }
    }
    else if (heuristicMethod == "mcs") {
        std::vector<short int> startingVertices;
        for(short int i{0}; i < graph.getNVertices();i++) startingVertices.push_back(i);

        // OUTPUT
        // for (auto &elem : startingVertices) std::cout << "(" << graph.getVertex(elem).label << "," << graph.getVertex(elem).activeNeighbours << ") ";
        // std::cout << std::endl;

        // start elimination
        for(int i{0}; i < graph.getNVertices();i++) 
        {
            int eliminatedID = startingVertices[0];
            startingVertices.erase(startingVertices.begin());
            eliminationOrdering.insert(eliminationOrdering.begin(), eliminatedID);

            Vertex eliminatedVertex = graph.getVertex(eliminatedID);
            const std::vector<short int> &neighbours = eliminatedVertex.getNeighbours();

            for(const auto &neighbourID : neighbours){
                Vertex& neighbour = graph.getVertex(neighbourID);
                if (!(neighbour.eliminated)) neighbour.activeNeighbours++;
            }


            // perform checkup sort
            std::sort(startingVertices.begin(),startingVertices.end(),[&graph,&eliminationSorts](int first, int second){bool stays = (graph.getVertex(first).activeNeighbours > graph.getVertex(second).activeNeighbours); if (!stays) eliminationSorts++; return stays;});

            // Generating OUTPUT
            // std::cout << graph.getVertex(eliminatedID).label ;
            // for (auto &elem : startingVertices) std::cout << "(" << graph.getVertex(elem).label << "," << graph.getVertex(elem).activeNeighbours << ") ";
            // std::cout << eliminationSorts << std::endl;
        }
    }

    // printf("INITIAL GRAPH: \n");
    // graph.print();
    std::cout<< "generated sorting" << std::endl;

    // OUTPUT
    // std::cout << "EliminationSorts: " << eliminationSorts << std::endl;
    // for (auto &elem : eliminationOrdering) std::cout << graph.getVertex(elem).ID() << " ";
    // std::cout << std::endl;

    // generate decomposition
    TreeDecomposition resultingTD = decomposition_from_ordering(graph, eliminationOrdering);

    // write output of tree decomp
    // resultingTD.print();

    // determine width & output it
    std::cout << "width: " << resultingTD.getWidth() << std::endl;

    // output times

    return 0;
}