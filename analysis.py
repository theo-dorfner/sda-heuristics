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

gen_writeERGraph(200,1/4)

# degreeResults = [[] * 3] * 3
# fillResults = [[] * 3] * 3
# mcsResults = [[] * 3] * 3
# print(degreeResults)

# # loop through everything
# for i,n in enumerate([10,100,500]):
#     for j,p in enumerate([1/4, 1/2, 3/4]):
#         fillTemp = []
#         degreeTemp = []
#         mcsTemp = []
#         for k in range(5):
#             print(f"n: {n}, p: {p} - {k}" + 10 * " ",end="\r")
#             gen_writeERGraph(n,p)

#             # fill
#             fillReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'fill']).decode("utf-8").split(":")
#             if(fillReturn[0] == "width"):
#                 fillTemp.append(int(fillReturn[1]))
#             else: print("bad result")

#             # degree
#             degreeReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'degree']).decode("utf-8").split(":")
#             if(degreeReturn[0] == "width"):
#                 degreeTemp.append(int(degreeReturn[1]))
#             else: print("bad result")

#             # mcs
#             mcsReturn = sp.check_output(['./build/heuristics', 'graph.txt', 'mcs']).decode("utf-8").split(":")
#             if(mcsReturn[0] == "width"):
#                 mcsTemp.append(int(mcsReturn[1]))
#             else: print("bad result")
#         degreeResults[i].append((np.average(degreeTemp), np.median(degreeTemp)))
#         fillResults[i].append((np.average(fillTemp), np.median(fillTemp)))
#         mcsResults[i].append((np.average(mcsTemp), np.median(mcsTemp)))

# print(degreeResults)
# print(fillResults)
# print(mcsResults)
