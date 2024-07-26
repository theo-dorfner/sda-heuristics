import matplotlib.pyplot as plt
import numpy as np
import json


file = np.load("results/widthResults.npz") # load results


# one plot for each pair of heuristics
comparisonPairs = [
    ("fill", "degree"),
    ("fill", "mcs"),
    ("degree", "mcs")
]

comparisonPairs = []

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



# --------------------------------------------------------------------------- MEDIAN AND AVERAGE ---------------------------------------------------------------------------



# CALCULATE MEDIAN AND AVERAGE

# results for respective measures
methodResults = {"fill": {"avg": [], "median": []}, 
                 "degree": {"avg": [], "median": []}, 
                 "mcs": {"avg": [], "median": []}}

xValues = [0.25,0.5,0.75]           # values for x-axis labels (p values from ER network)

# calculate averages
for i,n in enumerate(file["nValues"]):
    for j, p in enumerate(file["pValues"]):
        for method in ["fill", "degree", "mcs"]:
            data = file[method][i,j]
            methodResults[method]["avg"].append(np.average(data))
            methodResults[method]["median"].append(np.median(data))




# PLOT MEDIAN AND AVERAGE

# set up plot and large title/label
fig, (high, medium, low) = plt.subplots(nrows=3, ncols=2, sharex=True,figsize=(11, 9))
fig.suptitle(f"Comparison of Average and Median Width\nbetween Heuristic Methods",size=20)
fig.supylabel("Width",size=15)        # label heuristics method on y axis

# plot values for n=1000
high[0].plot(xValues, methodResults["fill"]["avg"][-3:],"o-",color="goldenrod", label= "Min Fill-In (fill)")
high[1].plot(xValues, methodResults["fill"]["median"][-3:],"v-",color="goldenrod")
high[0].plot(xValues, methodResults["degree"]["avg"][-3:],"o-",color="steelblue", label= "Min Degree (degree)")
high[1].plot(xValues, methodResults["degree"]["median"][-3:],"v-",color="steelblue")
high[0].plot(xValues, methodResults["mcs"]["avg"][-3:],"o-",color="tomato", label= "Max Cardinality (mcs)")
high[1].plot(xValues, methodResults["mcs"]["median"][-3:],"v-",color="tomato")

# plot values for n=100
medium[0].plot(xValues, methodResults["fill"]["avg"][3:-3],"o-",color="goldenrod")
medium[1].plot(xValues, methodResults["fill"]["median"][3:-3],"v-",color="goldenrod")
medium[0].plot(xValues, methodResults["degree"]["avg"][3:-3],"o-",color="steelblue")
medium[1].plot(xValues, methodResults["degree"]["median"][3:-3],"v-",color="steelblue")
medium[0].plot(xValues, methodResults["mcs"]["avg"][3:-3],"o-",color="tomato")
medium[1].plot(xValues, methodResults["mcs"]["median"][3:-3],"v-",color="tomato")

# plot values for n=10
low[0].plot(xValues, methodResults["fill"]["avg"][:3],"o-",color="goldenrod")
low[1].plot(xValues, methodResults["fill"]["median"][:3],"v-",color="goldenrod")
low[0].plot(xValues, methodResults["degree"]["avg"][:3],"o-",color="steelblue")
low[1].plot(xValues, methodResults["degree"]["median"][:3],"v-",color="steelblue")
low[0].plot(xValues, methodResults["mcs"]["avg"][:3],"o-",color="tomato")
low[1].plot(xValues, methodResults["mcs"]["median"][:3],"v-",color="tomato")

# set x-axis labels
low[0].set_xticks(xValues)
low[1].set_xticks(xValues)
low[0].set_xlabel("p value",size="large",labelpad=15)
low[1].set_xlabel("p value",size="large",labelpad=15)

# label average and median columns
high[0].set_title("Average",size=15, pad= 20)
high[1].set_title("Median",size=15, pad= 20)

# set n value labels for each row
high[0].set_ylabel(f"n=1000",size="large",labelpad=45, rotation = 0) 
medium[0].set_ylabel(f"n=100",size="large",labelpad=45, rotation = 0) 
low[0].set_ylabel(f"n=10",size="large",labelpad=45, rotation = 0) 

# create legend for the full figure
handles, labels = high[0].get_legend_handles_labels()
fig.legend(handles, labels, loc='upper right', borderpad= 1, bbox_to_anchor=(0.997, 0.997))

plt.subplots_adjust(left=0.17, top= 0.84, bottom=0.1)           # adjust margins

plt.savefig("plots/avg_median.png",dpi=300)