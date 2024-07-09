import networkx as nx
import subprocess as sp
import numpy as np


def gen_writeERGraph(n,p):
    graph = nx.erdos_renyi_graph(n,p)

    with open("graph.txt","w") as file:
        nEdges = len(list(graph.edges))
        for i,e in enumerate(graph.edges):
            file.write(f"{e[0]},{e[1]},")
            if (i < nEdges - 1):
                file.write("\n")

# gen_writeERGraph(800,2/4)

degreeResults = [[] * 3] * 3
fillResults = [[] * 3] * 3
mcsResults = [[] * 3] * 3
# print(degreeResults)

# loop through everything
for i,n in enumerate([1000,10,10]):
    for j,p in enumerate([1/4, 1/2, 3/4]):
        fillTemp = []
        degreeTemp = []
        mcsTemp = []
        for k in range(100):
            gen_writeERGraph(n,p)

            # print(f"n: {n}, p: {p} - {k}: fil" + 10 * " ",end="\r")
            # # fill
            # fillReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'fill']).decode("utf-8").split(":")
            # if(fillReturn[0] == "width"):
            #     fillTemp.append(int(fillReturn[1]))
            # else: print("bad result:",fillReturn)

            print(f"n: {n}, p: {p} - {k}: deg" + 10 * " ",end="\r")
            # degree
            degreeReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'degree']).decode("utf-8").split(":")
            if(degreeReturn[0] == "width"):
                degreeTemp.append(int(degreeReturn[1]))
            else: print("bad result:",degreeReturn)

            print(f"n: {n}, p: {p} - {k}: mcs" + 10 * " ",end="\r")
            # mcs
            mcsReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'mcs']).decode("utf-8").split(":")
            if(mcsReturn[0] == "width"):
                mcsTemp.append(int(mcsReturn[1]))
            else: print("bad result:",mcsReturn)
        degreeResults[i].append((np.average(degreeTemp), np.median(degreeTemp)))
        fillResults[i].append((np.average(fillTemp), np.median(fillTemp)))
        mcsResults[i].append((np.average(mcsTemp), np.median(mcsTemp)))

np.savez("results/avgMedian.npz",degree=degreeResults, fill=fillResults, mcs=mcsResults)

# print(degreeResults)
# print(fillResults)
# print(mcsResults)
