'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

# Set up modules and packages.
# Plotting
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
# from matplotlib import rcParams
# from matplotlib import cm
# from matplotlib.font_manager import FontProperties
# from mpl_toolkits.mplot3d import Axes3D
# from mpl_toolkits.mplot3d import axes3d
# import matplotlib.animation as animation

# I/O
# import commentjson
# import json
# from pprint import pprint
# import sqlite3

# Numerical
# import math
import numpy as np
import pandas as pd

# System
# import sys
import time
import os 

# Locate the string 
from csv_functions import stringLocator
from csv_functions import requiredFiles

# plt.style.use('dark_background')
plt.style.use('seaborn-whitegrid')

print ""
print "------------------------------------------------------------------"
print "                             dss_adcs                             "
print "                              0.1.0                               "
print "      Copyright (c) 2018, S.D. Cherukuri (srikarad007@gmail.com)  "
print "------------------------------------------------------------------"
print ""

# Start timer.
start_time = time.time( )

print ""
print "******************************************************************"
print "                          Input parameters                        "
print "******************************************************************"
print ""

# Hover over the point to get coordinates. 
def update_annot(ind, sc, annot):
    pos = sc.get_offsets()[ind["ind"][0]]
    annot.xy = pos
    text = "{}".format( " ".join([names[n] for n in ind["ind"]]))
    annot.set_text(text)
    annot.get_bbox_patch().set_facecolor(cmap(norm(c[ind["ind"][0]])))
    annot.get_bbox_patch().set_alpha(0.4)

def hover(event):
    vis = annot.get_visible()
    if event.inaxes == ax:
        cont, ind = sc.contains(event)
        if cont:
            update_annot(ind, sc, annot)
            annot.set_visible(True)
            fig.canvas.draw_idle()
        else:
            if vis:
                annot.set_visible(False)
                fig.canvas.draw_idle()

norm    = plt.Normalize(1,4)
cmap    = plt.cm.RdYlGn

# Change the font size 
font = {'family' : 'monospace',
        'weight' : 'bold',
        'size'   :  15}
matplotlib.rc('font', **font)        
matplotlib.rc('xtick', labelsize=15)
matplotlib.rc('ytick', labelsize=15)

figureSize = (8.0,6.0)

# Update this part to use the json input for plotting. 
path                = "/home/superman/Desktop/bulk_simulation"
saveFigPath         = "/home/superman/Desktop/bulk_simulation/figures_temp/"

stringToSearchWith          = 'state_history'
metadatString               = 'metadata'
# numberOfReactionWheels      = '4.csv'
# filesForTheplots            = requiredFiles( path, stringToSearchWith, numberOfReactionWheels)
filesForTheplots            = requiredFiles( path, stringToSearchWith )
print("Test: ", requiredFiles( path, metadatString) )

# print("Files locations: ", filesForTheplots)

mass                   = np.array([])
peakPower              = np.array([])
volume                 = np.array([])
settlingTime           = np.array([])
avgPower               = np.array([])

peakPowerGiven         = np.array([150.0, 170.0])
settlingTimeGiven      = np.array([10.0, 15.0])
massGiven              = np.array([10.0, 15.0])
avgPowerGiven          = np.array([10.0, 12.0])

# Markers to differentiate. 
markers     = [ '.', '+', 'o', '^' ]
colors      = [ 'r', 'b', 'k', 'y' ]

#### <<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>## 
#### Change this so the array is sized automatically based on the    ## 
#### size of the array. 
peakPowerPercent        = []
peakMomentumPercent     = []
#### <<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>## 


## <<<<<<<<<<<<<<<<<<< To find the string >>>>>>>>>>>>>>>>>>>>>>>>>  ##
# \d for numericals 
# \ to be followed by special characters 
# * all instances

for filename in range(len(filesForTheplots)):

    state_history       = pd.read_csv( filesForTheplots[filename] )
    # print(filesForTheplots[filename])
    # Update the index of dataframe to be the concept identifier
    state_history.set_index('ConceptIdentifier', inplace=True)
    
    # Locate the concept identifiers for all the concepts
    conceptIdentity                 = 'concept_3_*\\d\_\d\d'
    conceptIdentityStrings          = stringLocator(filesForTheplots[filename], conceptIdentity)
    # print("Identity strings: ", conceptIdentityStrings)
    # Locate the strings for necessary attributes
    stringToBeLocated               = 'rwPeakPower\\d'
    stringToBeLocated2              = 'rwMomentumPercent\\d'
    peakPowerPercentStrings         = stringLocator(filesForTheplots[filename], stringToBeLocated )
    peakMomentumPercentStrings      = stringLocator(filesForTheplots[filename], stringToBeLocated2 )
    
    mass                                = np.append( mass, state_history['mass'] )
    peakPower                           = np.append( peakPower, state_history['systemPeakPower'] )
    avgPower                            = np.append( avgPower, state_history['systemAvgPower'] )
    volume                              = np.append( volume, state_history['volume'])
    settlingTime                        = np.append( settlingTime, state_history['settlingTime'])
    reactionWheelPeakPowerPercent       = np.array([ ])
    reactionWheelPeakMomentumPercent    = np.array([ ])   
    
    # Get peak power for each reaction wheel:
    for iterator in range(len(peakPowerPercentStrings)): 
        tempPeakPowerPercent            = state_history[peakPowerPercentStrings[iterator]] 
        tempPeakMomentumPercent         = state_history[peakMomentumPercentStrings[iterator]] 
        peakPowerPercent.append(tempPeakPowerPercent)
        peakMomentumPercent.append(tempPeakMomentumPercent)
        pass 
    pass 

rwPeakPowerPercent              = np.array(peakPowerPercent) 
rwPeakMomentumPercent           = np.array(peakMomentumPercent)

massRange                       = np.arange( np.min(mass), np.max(mass), 1.0 ); 
peakPowerRange                  = np.arange( np.min(peakPower), np.max(peakPower), 1.0 ); 
avgPowerRange                   = np.arange( np.min(avgPower), np.max(avgPower), 1.0 ); 
settlingTimeRange               = np.arange( np.min(settlingTime), np.max(settlingTime), 1.0 ); 

names                           = state_history.index.values

def chunks(l, n):
    """Yield successive n-sized chunks from l."""
    for i in xrange(0, len(l), n):
        yield l[i:i + n]

# print(rwPeakPowerPercent.shape)
print(rwPeakMomentumPercent[0].shape)

# print("Before sorting: ", rwPeakPowerPercent )

# fig             = plt.figure(figsize=figureSize)
# ax              = fig.add_subplot(111)

# ax.hlines(1, [0], 10, lw=2)
# ax.hlines( y=0.5, xmin=5.0, xmax=10.0, linestyle='--', color='k' )

# for ii in range(len(conceptIdentityStrings)): 

#         tempMax    = np.amax(rwPeakPowerPercent, axis=0)
#         tempMin    = np.amin(rwPeakPowerPercent, axis=0)
#         print(tempMax)
#         print(tempMin)
#         ax.vlines( x=mass, ymax=tempMax, ymin=tempMin, lw=2 )
#         ax.scatter( mass, tempMax, marker='o' )
#         ax.scatter( mass, tempMin, marker='o' )

#         pass 

# newDataFrame    = state_history.sort_values(by='rwPowerPercent1')
# print(newDataFrame)
# print(state_history)

fig      = plt.figure(figsize=figureSize)
ax       = fig.add_subplot(111)
new      = chunks(peakPower, 100)
# print(new)

newMass  = chunks(mass, 100)

testArray = []
massTest  = []
for i, j in zip(new, newMass):     
    testArray.append(i)
    massTest.append(j)
ax.boxplot(testArray, positions=np.average(massTest, axis=1)) 
# ax.errorbar( xPositions, peakPowerPercentMean, yerr=peakPowerPercentStd, fmt='o' )
# ax.set_xticklabels( ['Concept 1', 'Concept 2'] )
ax.set_title('Peak power distribution')
ax.set_xlabel('Mass [kg]')
ax.set_ylabel('Peak Power [W]')
fig.savefig(saveFigPath + "peakPowerDistributionVsMass.png")

fig      = plt.figure(figsize=figureSize)
ax       = fig.add_subplot(111)
new      = chunks(avgPower, 100)
# print(new)
testArray = []
for i in new:     
    testArray.append(i)
ax.boxplot(testArray, positions=np.average(massTest, axis=1))
ax.set_title('Average power distribution')
ax.set_xlabel('Mass [kg]')
ax.set_ylabel('Average Power [W]')
fig.savefig(saveFigPath + "avgPowerDistributionVsMass.png")

fig      = plt.figure(figsize=figureSize)
ax       = fig.add_subplot(111)
new      = chunks(settlingTime, 100)

testArray = []
for i in new:     
    testArray.append(i)
ax.boxplot(testArray, positions=np.average(massTest, axis=1))
ax.set_title('Settling time distribution')
ax.set_xlabel('Mass [kg]')
ax.set_ylabel('Maneuver time [min]')
fig.savefig(saveFigPath + "settlingTimeDistributionVsMass.png")

fig      = plt.figure(figsize=figureSize)
ax       = fig.add_subplot(111)
new1     = chunks(rwPeakMomentumPercent[0], 100)
new2     = chunks(rwPeakMomentumPercent[1], 100)
new3     = chunks(rwPeakMomentumPercent[2], 100)

testArray   = []
testArray2  = []
testArray3  = []

for i, j, k in zip(new1, new2, new3):     
    testArray.append(i)
    testArray2.append(j)
    testArray3.append(k)
ax.boxplot(testArray, positions=np.average(massTest, axis=1))
ax.boxplot(testArray2, positions=np.average(massTest, axis=1))
ax.boxplot(testArray3, positions=np.average(massTest, axis=1))

ax.set_title('Momentum percentage distribution')
ax.set_xlabel('Mass [kg]')
ax.set_ylabel('Momentum storage [%]')
fig.savefig(saveFigPath + "momentumStorageDistributionVsMass.png")

# print(np.average(massTest))
# ax.errorbar( xPositions, peakPowerPercentMean, yerr=peakPowerPercentStd, fmt='o' )
# ax.set_xticklabels( ['Concept 1', 'Concept 2'] )

# fig       = plt.figure(figsize=figureSize)
# ax        = fig.add_subplot(111)

# c        = np.random.randint(1,5,size=len(mass))
# sc = ax.scatter( mass, peakPower, s=100, c=c, cmap=cmap, norm=norm )
# ax.set_xlabel('Mass [kg]')
# ax.set_ylabel('Peak power [W]')
# ax.legend()
# ax.grid(linestyle='--', linewidth=0.25, color='black')
# ax.fill_between( massRange, peakPowerGiven[0], peakPowerGiven[1], facecolor='yellow', alpha=0.7)
# ax.fill_betweenx( peakPowerRange, massGiven[0], massGiven[1], facecolor='red', alpha=0.4)
# plt.tight_layout()   

# annot   = ax.annotate("", xy=(0,0), xytext=(20,20),textcoords="offset points",
#                     bbox=dict(boxstyle="round", fc="w"),
#                     arrowprops=dict(arrowstyle="->"))
# annot.set_visible(False)

# fig.canvas.mpl_connect("motion_notify_event", hover)
# fig.savefig(saveFigPath + "case2a_massVsPeakPower.png")

# fig2             = plt.figure(figsize=figureSize)
# axScatter2       = fig2.add_subplot(111)

# fig3             = plt.figure(figsize=figureSize)
# axScatter3       = fig3.add_subplot(111)

# fig4             = plt.figure(figsize=figureSize)
# axScatter4       = fig4.add_subplot(111)

# axScatter2.scatter( settlingTime, peakPower )
# axScatter2.set_xlabel('Settling time [min]')
# axScatter2.set_ylabel('Peak power [W]')
# axScatter2.legend()
# axScatter2.grid(linestyle='--', linewidth=0.25, color='black')
# axScatter2.fill_between( settlingTimeRange, peakPowerGiven[0], peakPowerGiven[1], facecolor='yellow', alpha=0.7)
# axScatter2.fill_betweenx( peakPowerRange, settlingTimeGiven[0], settlingTimeGiven[1], facecolor='red', alpha=0.4)
# plt.tight_layout()   
# fig2.savefig(saveFigPath + "case2a_settlingTimeVsPeakPower.png")

# axScatter3.scatter( settlingTime, mass )
# axScatter3.set_xlabel('Settling time [min]')
# axScatter3.set_ylabel('Mass [kg]')
# axScatter3.legend()
# axScatter3.grid(linestyle='--', linewidth=0.25, color='black')
# axScatter3.fill_between( settlingTimeRange, massGiven[0], massGiven[1], facecolor='yellow', alpha=0.7)
# axScatter3.fill_betweenx( massRange, settlingTimeGiven[0], settlingTimeGiven[1], facecolor='red', alpha=0.4)
# plt.tight_layout()
# fig3.savefig(saveFigPath + "case2a_settlingTimeVsmass.png")

# axScatter4.scatter( avgPower, peakPower )
# axScatter4.set_xlabel('Avg Power [W]')
# axScatter4.set_ylabel('Peak Power [W]')
# axScatter4.legend()
# axScatter4.grid(linestyle='--', linewidth=0.25, color='black')
# axScatter4.fill_between( avgPowerRange, peakPowerGiven[0], peakPowerGiven[1], facecolor='yellow', alpha=0.7)
# axScatter4.fill_betweenx( peakPowerRange, avgPowerGiven[0], avgPowerGiven[1], facecolor='red', alpha=0.4)
# plt.tight_layout()
# fig4.savefig(saveFigPath + "case2a_PeakVsAvgPower.png")

plt.show(True)

print "Figures generated successfully!"
print ""

# Stop timer
end_time = time.time( )

print ""
print "------------------------------------------------------------------"
print "                         Exited successfully!                     "
print "------------------------------------------------------------------"
print ""