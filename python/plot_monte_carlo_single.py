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

# Change the font size 
# Change the font size 
font = {'family' : 'monospace',
        'weight' : 'bold',
        'size'   :  15}
matplotlib.rc('font', **font)        
matplotlib.rc('xtick', labelsize=15)
matplotlib.rc('ytick', labelsize=15)

# Update this part to use the json input for plotting. 
path                = "/home/superman/Desktop/monte_carlo_single"
metadataFilePath    = "/home/superman/Desktop/metadata.csv"

saveFigPath         = "/home/superman/Delft/Thesis/thesis-report/Images/results/case_2/"
# saveFigPath3        = "/home/superman/Delft/Thesis/thesis-report/Images/results/"
saveFigPath3        = "/home/superman/Delft/Thesis/IAC/iac_2018/Images/results/"
# saveFigPath3        = "/home/superman/Delft/IAC/presentation/results/"

case_2_inertia      = False
case_3_inertia      = False
case_2_slewRate     = False   
case_3_slewRate     = False

figureSize = (8.0,6.0)

stringToSearchWith          = 'state_history'
filesForTheplots            = natural_sort( requiredFiles( path, stringToSearchWith ) )
settlingTime                = np.array([])

n_bins      = 30

for jj in range(len(filesForTheplots)): 
    settlingTime        = np.append( settlingTime, calculateSettlingTime( filesForTheplots[jj]) )    
    pass 

peakpower, avgpower, maxPowerRws, avgPowerRws  = calculatePower( filesForTheplots ) 
# Fix this! 
hardwarePeakPower  = 7.0 

# peakPowerPercents   = np.array( [peakpower[:,1]])
# print(peakpower)

maxReactionWheelTorque, avgReactionWheelTorque, maxMomentum, avgMomentum, maxVelocity, avgVelocity  = extractConceptAttributes( filesForTheplots )
# peakPowerPercents   = calculatePeakPowerPercentage( metadataFilePath, filesForTheplots )
# print(peakPowerPercents)
peakMomentumPercent = calculateSaturationPercentage( metadataFilePath, filesForTheplots )

metadata                    = pd.read_csv( '/home/superman/Desktop/metadata.csv' ) 
momentsOfInertia            = np.array([metadata['principleInertia1'], metadata['principleInertia2'], metadata['principleInertia3']])
initialAttitude             = np.array([metadata['initialAttitude1'], metadata['initialAttitude2'], metadata['initialAttitude3']])
slewRate                    = np.array([metadata['slewRate']])

## Temporary plot ## 
left, width = 0.1, 0.65
bottom, height = 0.1, 0.65
bottom_h = left_h = left + width + 0.02

rect_scatter = [left, bottom, width, height]
rect_histx = [left, bottom_h, width, 0.2]
rect_histy = [left_h, bottom, 0.2, height]

fig     = plt.figure(figsize=figureSize)
# fig     = plt.figure()
axScatter = fig.add_subplot(111)
# plt.figure(1, figsize=(8, 8))
# axScatter = plt.axes(rect_scatter)
# axHistx = plt.axes(rect_histx)
# axHisty = plt.axes(rect_histy)
axScatter.scatter( momentsOfInertia[1], peakpower, color='g', marker='.', label='RW1', linewidth=10)
# axHistx.hist(momentsOfInertia[0])
# axHisty.hist(peakpower, orientation='horizontal')

axScatter.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
axScatter.set_ylabel('Peak power [W]')
axScatter.grid(linestyle='--', linewidth=0.25, color='black')
plt.tight_layout()
fig.savefig(saveFigPath3 + "peak_power.eps")

## End of temporary plot ## 

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
fig     = plt.figure(figsize=figureSize)
# fig     = plt.figure()
ax1     = fig.add_subplot(111)

ax1.scatter( momentsOfInertia[1], maxPowerRws[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# ax1.legend(loc=2)
ax1.legend()
ax1.scatter( momentsOfInertia[1], maxPowerRws[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# ax1.legend(loc=2)
ax1.legend()
ax1.scatter( momentsOfInertia[1], maxPowerRws[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# ax1.legend(loc=2)
ax1.legend()
ax1.scatter( momentsOfInertia[1], maxPowerRws[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# ax1.legend(loc=2)
ax1.legend()
ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
ax1.set_ylabel('Peak power [W]')
ax1.grid(linestyle='--', linewidth=0.25, color='black')
plt.tight_layout()
plt.grid()
plt.close(False)
fig.savefig(saveFigPath3 + "peak_power_case3.eps")

# Maximum momentum w.r.t moments of inertia. 
fig     = plt.figure(figsize=figureSize)
# fig     = plt.figure()
ax1     = fig.add_subplot(111)

ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
ax1.scatter( momentsOfInertia[1], peakMomentumPercent[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
ax1.set_ylabel('Peak / Total momentum [%]')
ax1.grid(linestyle='--', linewidth=0.25, color='black')
plt.tight_layout()
# plt.grid()
plt.close(False)
fig.savefig(saveFigPath3 + "peak_momentum_percent.eps")

# Peak power percentage w.r.t moments of inertia. 
# fig     = plt.figure(figsize=figureSize)
# fig     = plt.figure()
# ax1     = fig.add_subplot(111)

# ax1.scatter( momentsOfInertia[1], peakPowerPercents[:,0],  marker='+', color='g', label='RW1', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakPowerPercents[:,1],  marker='+', color='b', label='RW2', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakPowerPercents[:,2],  marker='+', color='y', label='RW3', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.scatter( momentsOfInertia[1], peakPowerPercents[:,3],  marker='+', color='r', label='RW4', linewidth=2)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
# ax1.set_ylabel('Peak / Total power [%]')
# ax1.grid(linestyle='--', linewidth=0.25, color='black')
# plt.tight_layout()
# plt.grid()
# plt.close(False)
# fig.savefig(saveFigPath3 + "peak_momentum_percent.eps")

# Temp reaction wheel momentum percentage model.
fig     = plt.figure(figsize=figureSize)
# fig     = plt.figure()
ax1     = fig.add_subplot(111)

ax1.scatter( momentsOfInertia[1], maxMomentum[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
ax1.scatter( momentsOfInertia[1], maxMomentum[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
ax1.scatter( momentsOfInertia[1], maxMomentum[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
ax1.scatter( momentsOfInertia[1], maxMomentum[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# ax1.legend(loc=2)
# ax1.legend()
# ax1.grid(linestyle='--', linewidth=0.25, color='black')

ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
ax1.set_ylabel('Peak momentum [Nms]')
plt.tight_layout()
ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
# plt.grid()
plt.close(False)
fig.savefig(saveFigPath3 + "peak_momentum.eps")

# Temp reaction maximum torque. 
fig     = plt.figure(figsize=figureSize)
# fig     = plt.figure()
ax1     = fig.add_subplot(111)

ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,0],  marker='.', color='g', label='RW1', linewidth=10)
# ax1.legend(loc=1)
ax1.legend()
ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,1],  marker='.', color='b', label='RW2', linewidth=10)
# ax1.legend(loc=1)
ax1.legend()
ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,2],  marker='.', color='y', label='RW3', linewidth=10)
# ax1.legend(loc=1)
ax1.legend()
ax1.scatter( momentsOfInertia[1], maxReactionWheelTorque[:,3],  marker='.', color='r', label='RW4', linewidth=10)
# ax1.legend(loc=1)
ax1.legend()
ax1.grid(linestyle='--', linewidth=0.25, color='black')
ax1.set_ylim(bottom=0)
ax1.set_xlabel('Moment of Inertia in Y axis [kg/m2]')
ax1.set_ylabel('Maximum torque [Nm]')
ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
plt.tight_layout()
# plt.grid()
plt.close(False)
fig.savefig(saveFigPath3 + "peak_torque.eps")

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