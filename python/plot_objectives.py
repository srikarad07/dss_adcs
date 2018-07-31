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
from mpl_toolkits.mplot3d import Axes3D
# from mpl_toolkits.mplot3d import axes3d
# import matplotlib.animation as animation
import matplotlib.lines as mlines

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
import functions
from functions import calculateSaturationTime
from functions import calculateSettlingTime 

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

# Update this part to use the json input for plotting. 
filePath                    =  "/home/superman/Desktop/bulk_simulation"
stateHistoryString          = 'state_history'
metadatFileString           = 'metadata'

metadataFiles               = requiredFiles( filePath, metadatFileString )

minReactionWheels           = 3 
maxReactionWheels           = 7

numberOfReactionWheels      = np.arange( minReactionWheels, maxReactionWheels )

colors                      = np.array( ['r', 'b', 'g', 'y'] )
markers                     = np.array( ["o", "^", "*", "+"] )

## <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Extract thsi from the metadata fiel >>>>>>>>>>>>>>>>>>>>>>>>>>>
referenceAttitude       = [0, 0, 0, 1]
angularVelocityGain     = 2 * 0.707 * 0.1 
slewRate                = np.deg2rad(0.2)
## <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

fig  = plt.figure()
ax   = fig.add_subplot(111, projection='3d')

fig2 = plt.figure()
ax2  = fig2.add_subplot(111)

fig3 = plt.figure()
ax3  = fig3.add_subplot(111)

fig4 = plt.figure()
ax4  = fig4.add_subplot(111)

for c, m, iterator in zip(range(len(colors)), range(len(markers)), range(len(numberOfReactionWheels)) ):
    
    stateHistoryFiles       = requiredFiles( filePath, stateHistoryString, str(numberOfReactionWheels[iterator]) + '.csv')
    metadata                = pd.read_csv( metadataFiles[iterator] )
    
    mass                    = metadata['mass']
    volume                  = metadata['volume']
    maxPower, avgPower      = functions.calculatePower( stateHistoryFiles )

    ax.scatter( mass, volume, maxPower, c=colors[c], marker=markers[m] )
    ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax.grid()

    settlingTime            = [ calculateSettlingTime(stateHistoryFiles[ii]) for ii in range(len(stateHistoryFiles)) ]
    saturationTime          = [ calculateSaturationTime(stateHistoryFiles[ii], referenceAttitude, angularVelocityGain, slewRate)[0] for ii in range(len(stateHistoryFiles)) ]

    performance             = functions.calculatePerformance( settlingTime, saturationTime )

    ax2.scatter( mass, performance, c=colors[c], marker=markers[m] )
    ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax2.grid()

    ax3.scatter( volume, performance, c=colors[c], marker=markers[m] )
    ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax3.grid()

    ax4.scatter( avgPower, performance, c=colors[c], marker=markers[m] )
    ax4.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax4.grid()

    pass 

rw3 = mlines.Line2D([], [], color='r', marker='o', linestyle='None',
                          markersize=10, label='3 reaction wheels concept')
rw4 = mlines.Line2D([], [], color='b', marker='^', linestyle='None',
                          markersize=10, label='4 reaction wheels concept')
rw5 = mlines.Line2D([], [], color='g', marker='*', linestyle='None',
                          markersize=10, label='5 reaction wheels concept')
rw6 = mlines.Line2D([], [], color='y', marker='+', linestyle='None',
                          markersize=10, label='6 reaction wheels concept') 

ax.set_xlabel('Mass [kg]')
ax.set_ylabel('Volume [m3]')
ax.set_zlabel('Max Power [W]')
plt.legend(handles=[rw3, rw4, rw5, rw6]) 

ax2.set_xlabel('Mass [kg]')
ax2.set_ylabel('Performance')
plt.legend(handles=[rw3, rw4, rw5, rw6])

ax3.set_xlabel('Volume [m3]')
ax3.set_ylabel('Performance')
plt.legend(handles=[rw3, rw4, rw5, rw6])

ax4.set_xlabel('Average Power [W]')
ax4.set_ylabel('Performance')
plt.legend(handles=[rw3, rw4, rw5, rw6])
# plt.close()

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