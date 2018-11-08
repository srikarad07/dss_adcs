'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

import re

def natural_sort(l): 
    convert = lambda text: int(text) if text.isdigit() else text.lower() 
    alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ] 
    return sorted(l, key = alphanum_key)

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

# Performance and cost functions 
from functions import calculatePower
from functions import calculateSettlingTime
from functions import extractConceptAttributes
from functions import calculatePeakPowerPercentage
from functions import calculateSaturationPercentage
# from functions import calculateMomentumStoragePercentage

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
## <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<, TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ## 
## Figure out a way to move it to a separate function ## 
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
## <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<, TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ## 

# Change the font size 
# Change the font size 
font = {'family' : 'monospace',
        'weight' : 'bold',
        'size'   :  15}
matplotlib.rc('font', **font)        
matplotlib.rc('xtick', labelsize=15)
matplotlib.rc('ytick', labelsize=15)

# Update this part to use the json input for plotting. 
path                = "/home/superman/Desktop/bulk_simulation"
saveFigPath         = "/home/superman/Desktop/bulk_simulation/figures_temp/"

case_2_inertia      = False
case_3_inertia      = False
case_2_slewRate     = False   
case_3_slewRate     = False

figureSize = (8.0,6.0)

stringToSearchWith          = 'state_history'
filesForTheplots            = natural_sort( requiredFiles( path, stringToSearchWith ) )
settlingTime                = np.array([])

metadataString              = 'metadata'
metadatafiles               = requiredFiles( path, metadataString )
print(metadatafiles)
print(filesForTheplots)
mass                   = np.array([])
peakPower              = np.array([])
volume                 = np.array([])
settlingTime           = np.array([])
avgPower               = np.array([])
slewRate               = np.array([])

#### <<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>## 
#### Change this so the array is sized automatically based on the    ## 
#### size of the array. 
peakPowerPercent        = []
peakMomentumPercent     = []
#### <<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>## 

for filename in range(len(filesForTheplots)):

    state_history       = pd.read_csv( filesForTheplots[filename] )
    metadata            = pd.read_csv( metadatafiles[filename] )

    # print(filesForTheplots[filename])
    # Update the index of dataframe to be the concept identifier
    state_history.set_index('ConceptIdentifier', inplace=True)
    metadata.set_index('ConceptIdentifier', inplace=True)

    # Locate the concept identifiers for all the concepts
    conceptIdentity                 = 'concept_3_*\\d\_\d\d'
    conceptIdentityStrings          = stringLocator(filesForTheplots[filename], conceptIdentity)
    
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
    momentsOfInertia                    = np.array([metadata['principleInertia1'], metadata['principleInertia2'], metadata['principleInertia3']])
    reactionWheelPeakPowerPercent       = np.array([ ])
    reactionWheelPeakMomentumPercent    = np.array([ ])   
    
    ## Metadata parameters
    slewRate                            = np.append( slewRate, metadata['SlewRate'])

    # Get peak power for each reaction wheel:
    for iterator in range(len(peakPowerPercentStrings)): 
        tempPeakPowerPercent            = state_history[peakPowerPercentStrings[iterator]] 
        tempPeakMomentumPercent         = state_history[peakMomentumPercentStrings[iterator]] 
        peakPowerPercent.append(tempPeakPowerPercent)
        peakMomentumPercent.append(tempPeakMomentumPercent)
        pass 
    pass 
names                   = state_history.index.values
n_bins                  = 30
print("Compare indexes: ", np.array_equal( state_history.index.values, metadata.index.values) )
# print("Moments: ", momentsOfInertia)
# print("test: ", peakPowerPercent[0], peakPowerPercent[1], peakPowerPercent[2],)
## Temporary plot ## 
left, width = 0.1, 0.65
bottom, height = 0.1, 0.65
bottom_h = left_h = left + width + 0.02

rect_scatter    = [left, bottom, width, height]
rect_histx      = [left, bottom_h, width, 0.2]
rect_histy      = [left_h, bottom, 0.2, height]
# plt.figure(1, figsize=(8, 8))
# ax = plt.axes(rect_scatter)
# axHistx = plt.axes(rect_histx)
# axHisty = plt.axes(rect_histy)
# axHistx.hist(momentsOfInertia[0])
# axHisty.hist(peakpower, orientation='horizontal')

fig     = plt.figure(figsize=figureSize)
ax      = fig.add_subplot(111)
sc      = ax.scatter( momentsOfInertia[1], peakPower, color='g', marker='.', label='RW1', linewidth=10)
c       = np.random.randint(1,5,size=len(mass))

ax.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
ax.set_ylabel('Peak power [W]')
ax.grid(linestyle='--', linewidth=0.25, color='black')
plt.tight_layout()

annot   = ax.annotate("", xy=(0,0), xytext=(20,20),textcoords="offset points",
                    bbox=dict(boxstyle="round", fc="w"),
                    arrowprops=dict(arrowstyle="->"))
annot.set_visible(False)

fig.canvas.mpl_connect("motion_notify_event", hover)
fig.savefig(saveFigPath + "peak_power.eps")

# ## End of temporary plot ## 


# ## ACS moment of inertias with respect to settling time ## 
# fig     = plt.figure(figsize=figureSize)
# axScatter = fig.add_subplot(111)

# axScatter.scatter( momentsOfInertia[1], settlingTime, color='g', marker='.', label='RW1', linewidth=10)
# # axHistx.hist(momentsOfInertia[0])
# # axHisty.hist(peakpower, orientation='horizontal')

# axScatter.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
# axScatter.set_ylabel('Settling time [min]')
# axScatter.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# fig.savefig(saveFigPath + "settlingTime.eps")

# # Peak power percentage w.r.t moments of inertia. 
# fig     = plt.figure(figsize=figureSize)
# # fig     = plt.figure()
# ax1     = fig.add_subplot(111)
# ax1.scatter( momentsOfInertia[1], peakPowerPercent[0],  marker='+', color='g', label='RW1', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakPowerPercent[1],  marker='+', color='b', label='RW2', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakPowerPercent[2],  marker='+', color='y', label='RW3', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# # ax1.scatter( momentsOfInertia[1], peakPowerPercent[3],  marker='+', color='r', label='RW4', linewidth=2)
# # ax1.legend(loc=2)
# ax1.legend()
# ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
# ax1.set_ylabel('Peak / Total power [%]')
# ax1.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# plt.grid()
# plt.close(False)
# fig.savefig(saveFigPath + "peak_power_rw.eps")

# ## Slew rate change 
# fig     = plt.figure(figsize=figureSize)
# # fig     = plt.figure()
# axScatter = fig.add_subplot(111)
# # plt.figure(1, figsize=(8, 8))
# # axScatter = plt.axes(rect_scatter)
# # axHistx = plt.axes(rect_histx)
# # axHisty = plt.axes(rect_histy)
# axScatter.scatter( slewRate, peakPower, color='g', marker='.', label='RW1', linewidth=10)
# # axHistx.hist(momentsOfInertia[0])
# # axHisty.hist(peakpower, orientation='horizontal')

# axScatter.set_xlabel('Slew Rate [deg/sec]')
# axScatter.set_ylabel('Peak power [W]')
# axScatter.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# # fig.savefig(saveFigPath + "slew_peak_power.eps")

# ## End of temporary plot ## 


# ## ACS moment of inertias with respect to settling time ## 
# fig     = plt.figure(figsize=figureSize)
# axScatter = fig.add_subplot(111)

# axScatter.scatter( slewRate, settlingTime, color='g', marker='.', label='RW1', linewidth=10)
# # axHistx.hist(momentsOfInertia[0])
# # axHisty.hist(peakpower, orientation='horizontal')

# axScatter.set_xlabel('Slew Rate [deg/sec]')
# axScatter.set_ylabel('Settling time [min]')
# axScatter.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# # fig.savefig(saveFigPath + "slew_settlingTime.eps")

# # Peak power percentage w.r.t moments of inertia. 
# fig     = plt.figure(figsize=figureSize)
# # fig     = plt.figure()
# ax1     = fig.add_subplot(111)
# ax1.scatter( slewRate, peakPowerPercent[0],  marker='+', color='g', label='RW1', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( slewRate, peakPowerPercent[1],  marker='+', color='b', label='RW2', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( slewRate, peakPowerPercent[2],  marker='+', color='y', label='RW3', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# # ax1.scatter( slewRate, peakPowerPercent[3],  marker='+', color='r', label='RW4', linewidth=2)
# # ax1.legend(loc=2)
# ax1.legend()
# ax1.set_xlabel('Slew Rate [deg/sec]')
# ax1.set_ylabel('Peak / Total power [%]')
# ax1.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# plt.grid()
# plt.close(False)
# fig.savefig(saveFigPath + "slew_peak_power_rw.eps")

# Reaction wheel peak power w.r.t to moments of inertia.

# left, width = 0.1, 0.65
# bottom, height = 0.1, 0.65
# bottom_h = left_h = left + width + 0.02

# rect_scatter = [left, bottom, width, height]
# rect_histx = [left, bottom_h, width, 0.2]
# rect_histy = [left_h, bottom, 0.2, height]

# plt.figure(1, figsize=figureSize)
# axScatter = plt.axes(rect_scatter)
# axHistx = plt.axes(rect_histx)
# axHisty = plt.axes(rect_histy)
# axScatter.scatter( momentsOfInertia[0], maxPowerRws[:,0], color='r', marker='o', label='RW1')
# axHistx.hist(momentsOfInertia[0])
# axHisty.hist(maxPowerRws[:,0], orientation='horizontal')

# fig     = plt.figure(figsize=figureSize)
# # fig     = plt.figure()
# ax1     = fig.add_subplot(111)

# ax1.scatter( momentsOfInertia[1], maxPowerRws[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# # ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxPowerRws[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# # ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxPowerRws[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# # ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxPowerRws[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# # ax1.legend(loc=2)
# ax1.legend()
# ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
# ax1.set_ylabel('Peak power [W]')
# ax1.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# plt.grid()
# plt.close(False)
# fig.savefig(saveFigPath3 + "peak_power_case3.eps")

# # Maximum momentum w.r.t moments of inertia. 
# fig     = plt.figure(figsize=figureSize)
# # fig     = plt.figure()
# ax1     = fig.add_subplot(111)

# ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
# ax1.set_ylabel('Peak / Total momentum [%]')
# ax1.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# # plt.grid()
# plt.close(False)
# fig.savefig(saveFigPath3 + "peak_momentum_percent.eps")

# # Temp reaction wheel momentum percentage model.
# fig     = plt.figure(figsize=figureSize)
# # fig     = plt.figure()
# ax1     = fig.add_subplot(111)

# ax1.scatter( momentsOfInertia[1], maxMomentum[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxMomentum[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxMomentum[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxMomentum[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# # ax1.legend(loc=2)
# # ax1.legend()
# # ax1.grid(linestyle='--', linewidth=0.25, color='black')

# ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
# ax1.set_ylabel('Peak momentum [Nms]')
# plt.tight_layout()
# ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
# # plt.grid()
# plt.close(False)
# fig.savefig(saveFigPath3 + "peak_momentum.eps")

# # Temp reaction maximum torque. 
# fig     = plt.figure(figsize=figureSize)
# # fig     = plt.figure()
# ax1     = fig.add_subplot(111)

# ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# # ax1.legend(loc=1)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# # ax1.legend(loc=1)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# # ax1.legend(loc=1)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# # ax1.legend(loc=1)
# ax1.legend()
# ax1.grid(linestyle='--', linewidth=0.25, color='black')
# ax1.set_ylim(bottom=0)
# ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
# ax1.set_ylabel('Maximum torque [Nm]')
# ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
# plt.tight_layout()
# # plt.grid()
# plt.close(False)
# fig.savefig(saveFigPath3 + "peak_torque.eps")

plt.show()

print "Figures generated successfully!"
print ""

# Stop timer
end_time = time.time( )

print ""
print "------------------------------------------------------------------"
print "                         Exited successfully!                     "
print "------------------------------------------------------------------"
print ""