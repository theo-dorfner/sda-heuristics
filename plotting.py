import matplotlib.pyplot as plt
import numpy as np


file = np.load("results/widthResults.npz") # load results


# one plot for each pair of heuristics
comparisonPairs = [
    ("fill", "degree"),
    ("fill", "mcs"),
    ("degree", "mcs")
]


# dict for resolving tag to full heuristic name
fullName = {"fill": "Min-Fill", "degree": "Min-Degree", "mcs": "Max-Cardinality"}



for pair in comparisonPairs:

    # set up plot for the comparison pair
    fig, subplots = plt.subplots(3,3,figsize=(11, 9))
    fig.suptitle(f"Comparison between {fullName[pair[0]]} and {fullName[pair[1]]}",size=20)
    fig.supxlabel(fullName[pair[0]],size=15)        # label heuristics method on x axis
    fig.supylabel(fullName[pair[1]],size=15)        # label heuristics method on y axis

    # add subplots
    for i,n in enumerate(file["nValues"]):
        for j, p in enumerate(file["pValues"]):
            
            # load data
            xData = file[pair[0]][i,j]
            yData = file[pair[1]][i,j]

            # extract unique points and their counts
            datapoints = [[xData[i],yData[i]] for i in range(len(xData))]
            uniquePoints, counts = np.unique(np.array(datapoints),axis=0,return_counts=True)

            # get min and max for plotting the diagonal
            max = np.min([np.max(xData), np.max(yData)])
            min = np.max([np.min(xData), np.min(yData)])
            if(max == min):
                max = int(np.average([np.max(xData), np.max(yData)]))
                min = int(np.average([np.min(xData), np.min(yData)]))
            subplots[i,j].plot([min,max],[min,max],color="tomato",zorder=1,alpha=0.9,linewidth=0.5)

            # scatterplot of unique points that scale in size with their counts
            subplots[i,j].scatter(uniquePoints[:,0],uniquePoints[:,1],s=counts *10,alpha=1,linewidths=0,zorder=10,c="steelblue")


            # plot settings
            subplots[i,j].set_yticks(np.linspace(np.min(yData),np.max(yData),5,dtype=np.int64))
            subplots[i,j].set_box_aspect(1)

            # ensure bottom most tick labels are visible by rotating them
            if(i == 2):
                subplots[i,j].set_xticks(np.linspace(np.min(xData),np.max(xData),5,dtype=np.int64))
                subplots[i,j].set_xticklabels(np.linspace(np.min(xData),np.max(xData),5,dtype=np.int64),rotation=45)
            else:
                subplots[i,j].set_xticks(np.linspace(np.min(xData),np.max(xData),5,dtype=np.int64))
                subplots[i,j].set_xticklabels(np.linspace(np.min(xData),np.max(xData),5,dtype=np.int64))

            # set n and p labels for rows/columns
            if(j == 0):
                subplots[i,j].set_ylabel(f"n={n}",size="large",labelpad=45, rotation = 0) 
            if(i == 0):
                subplots[i,j].set_title(f"p={p}",size="large", pad=23)


    # adjust border sizes and save
    plt.subplots_adjust(left=0.1,
                    bottom=0.1, 
                    right=1, 
                    top=0.88, 
                    wspace=-0.2, 
                    hspace=0.2)
    plt.savefig(f"plots/comparison_{pair[0]}-{pair[1]}.png",dpi=300)