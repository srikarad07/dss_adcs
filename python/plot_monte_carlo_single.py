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

# Performance and cost functions 
from functions import calculatePower
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
path                = "/home/superman/Desktop/monte_carlo_single"

stringToSearchWith          = 'state_history'
filesForTheplots            = requiredFiles( path, stringToSearchWith )

peakpower, avgpower         = calculatePower( filesForTheplots ) 

# print("Peak power: ", peakpower )
# print("Avergae power: ", avgpower )

# figure, ax                  = plt.subplots(111)
n, bins, patches            = plt.hist( peakpower )
# plt.show() 

settlingTimeArray = []

# Calculate settling time 
for ii in range(len(filesForTheplots)): 
    tempSettlingTime    = calculateSettlingTime( filesForTheplots[ii] )
    settlingTimeArray   = np.append( settlingTimeArray, tempSettlingTime )
    pass 

n, bins, patches        = plt.hist(settlingTimeArray)
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