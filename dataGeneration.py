import networkx as nx
import subprocess as sp
import numpy as np


def gen_writeERGraph(n,p):
    graph = nx.erdos_renyi_graph(n,p)       # generate graph

    # store graph
    with open("graph.txt","w") as file:
        nEdges = len(list(graph.edges))
        for i,e in enumerate(graph.edges):
            file.write(f"{e[0]},{e[1]},")
            if (i < nEdges - 1):
                file.write("\n")



degreeResults = []
fillResults = []
mcsResults = []

nVals = [10,100,1000]
pVals = [1/4, 1/2, 3/4]
nRuns = [100,100,100]



# loop through problem sizes
for i,n in enumerate(nVals):

    # temporary lists for collecting configuration results
    fillTemp0 = []
    degreeTemp0 = []
    mcsTemp0 = []

    # loop through all p values
    for j,p in enumerate(pVals):

        # temporary lists for collecting results for one n value
        fillTemp1 = []
        degreeTemp1 = []
        mcsTemp1 = []

        for k in range(nRuns[i]):

            gen_writeERGraph(n,p)                                               # generate and store graph


            # fill
            print(f"n: {n}, p: {p} - {k}: fil" + 10 * " ",end="\r")             # update status print
            fillReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'fill']).decode("utf-8").split(":")
            if(fillReturn[0] == "width"):
                fillTemp1.append(int(fillReturn[1]))
            else: print("bad result:", fillReturn)


            # degree
            print(f"n: {n}, p: {p} - {k}: deg" + 10 * " ",end="\r")             # update status print
            degreeReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'degree']).decode("utf-8").split(":")
            if(degreeReturn[0] == "width"):
                degreeTemp1.append(int(degreeReturn[1]))
            else: print("bad result:",degreeReturn)


            # mcs
            print(f"n: {n}, p: {p} - {k}: mcs" + 10 * " ",end="\r")             # update status print
            mcsReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'mcs']).decode("utf-8").split(":")
            if(mcsReturn[0] == "width"):
                mcsTemp1.append(int(mcsReturn[1]))
            else: print("bad result:",mcsReturn)

        # append results from one configuration
        fillTemp0.append(degreeTemp1)
        degreeTemp0.append(fillTemp1)
        mcsTemp0.append(mcsTemp1)

    # append results from one n value
    fillResults.append(fillTemp0)
    degreeResults.append(degreeTemp0)
    mcsResults.append(mcsTemp0)



# save results
np.savez("results/widthResults.npz",degree=degreeResults, fill=fillResults, mcs=mcsResults, nValues=nVals, pValues=pVals,runNumbers=nRuns)


# print(degreeResults)
# print(fillResults)
# print(mcsResults)