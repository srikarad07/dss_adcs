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

plt.style.use('dark_background')

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

saveFigPath         = "/home/superman/Delft/Thesis/thesis-report/Images/results/case_2/"
saveFigPath2        = "/home/superman/Delft/Thesis/thesis-report/Images/results/case_3/"

case_2_inertia      = False
case_3_inertia      = False
case_2_slewRate     = False   
case_3_slewRate     = False

figureSize          = [8,5]

stringToSearchWith          = 'state_history'
filesForTheplots            = natural_sort( requiredFiles( path, stringToSearchWith ) )
settlingTime                = np.array([])

n_bins      = 30

for jj in range(len(filesForTheplots)): 
    settlingTime        = np.append( settlingTime, calculateSettlingTime( filesForTheplots[jj]) )    
    pass 
peakpower, avgpower, maxPowerRws, avgPowerRws  = calculatePower( filesForTheplots ) 
maxReactionWheelTorque, avgReactionWheelTorque, maxMomentum, avgMomentum, maxVelocity, avgVelocity  = extractConceptAttributes( filesForTheplots )
# print(maxReactionWheelTorque.shape)
metadata                    = pd.read_csv( '/home/superman/Desktop/metadata.csv' ) 
momentsOfInertia            = np.array([metadata['principleInertia1'], metadata['principleInertia2'], metadata['principleInertia3']])
initialAttitude             = np.array([metadata['initialAttitude1'], metadata['initialAttitude2'], metadata['initialAttitude3']])
slewRate                    = np.array([metadata['slewRate']])

# # Case3: Moments on inertia Peak power
fig  = plt.figure()
ax1  = fig.add_subplot(111)

# # Case3: Moments of inertia average power 
fig2   = plt.figure()
ax2    = fig2.add_subplot(111)

## Case 3 histogram peak power and average power. 
fig3   = plt.figure()
fig4   = plt.figure()
ax3    = fig3.add_subplot(111)
ax4    = fig4.add_subplot(111)

ax1.scatter(  momentsOfInertia[0], peakpower, color='r', marker='o' )
ax1.set_xlabel('J_{xx} [kg/m^2]')
ax1.set_ylabel('Peak power [W]')
plt.close()

ax2.scatter( momentsOfInertia[0], avgpower, color='r', marker='o' )
ax2.set_xlabel('J_{xx} [kg/m^2]')
ax2.set_ylabel('Average power [W]')
plt.close()

ax3.hist( peakpower, bins=n_bins )
ax3.set_xlabel('Peak Power [W]')
ax3.set_ylabel('Count')
plt.close()

ax4.hist( avgpower, bins=n_bins )
ax4.set_xlabel('Average Power [W]')
ax4.set_ylabel('Count')
plt.close()

if case_3_inertia == True: 
    fig.savefig(saveFigPath2 + 'case3_moi_vs_peak_power.eps')
    fig2.savefig(saveFigPath2 + 'case3_moi_vs_avg_power.eps')
    fig3.savefig(saveFigPath2 + 'case3_peak_power_hist.eps')
    fig4.savefig(saveFigPath2 + 'case3_avg_power_hist.eps')   
    print("Inertia: Enters this loop") 
    pass 

# # Case3: Slew rate Peak power
fig  = plt.figure()
ax1  = fig.add_subplot(111)

# # Case3: Slew rate average power 
fig2   = plt.figure()
ax2    = fig2.add_subplot(111)

## Case 3 histogram peak power and average power. 
fig3   = plt.figure()
fig4   = plt.figure()
ax3    = fig3.add_subplot(111)
ax4    = fig4.add_subplot(111)

ax1.scatter(  slewRate, peakpower, color='r', marker='o' )
ax1.set_xlabel('Slew Rate [deg/sec]')
ax1.set_ylabel('Peak power [W]')
plt.close()

ax2.scatter( slewRate, avgpower, color='r', marker='o' )
ax2.set_xlabel('Slew Rate [deg/sec]')
ax2.set_ylabel('Average power [W]')
plt.close()

ax3.hist( peakpower, bins=n_bins )
ax3.set_xlabel('Peak Power [W]')
ax3.set_ylabel('Count')
plt.close()

ax4.hist( avgpower, bins=n_bins )
ax4.set_xlabel('Average Power [W]')
ax4.set_ylabel('Count')
plt.close()

## Temporary plot ## 
left, width = 0.1, 0.65
bottom, height = 0.1, 0.65
bottom_h = left_h = left + width + 0.02

rect_scatter = [left, bottom, width, height]
rect_histx = [left, bottom_h, width, 0.2]
rect_histy = [left_h, bottom, 0.2, height]

fig     = plt.figure()
axScatter = fig.add_subplot(111)
# plt.figure(1, figsize=(8, 8))
# axScatter = plt.axes(rect_scatter)
# axHistx = plt.axes(rect_histx)
# axHisty = plt.axes(rect_histy)
axScatter.scatter( momentsOfInertia[0], peakpower, color='g', marker='o', label='RW1')
# axHistx.hist(momentsOfInertia[0])
# axHisty.hist(peakpower, orientation='horizontal')

axScatter.set_xlabel('Moment of Inertia in X axis [kg/m2]')
axScatter.set_ylabel('Peak power [W]')

## End of temporary plot ## 

if case_3_slewRate == True: 
    fig.savefig(saveFigPath2 + 'case3_slewRate_vs_peak_power.eps')
    fig2.savefig(saveFigPath2 + 'case3_slewRate_vs_avg_power.eps')
    fig3.savefig(saveFigPath2 + 'case3_slew_peak_power_hist.eps')
    fig4.savefig(saveFigPath2 + 'case3_slew_avg_power_hist.eps')    
    pass 

# Case 2: Plot histograms for reaction wheel peak power. 
fig     = plt.figure( figsize=figureSize )
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.hist(maxPowerRws[:,0], bins=n_bins, label='RW1')
ax1.legend()
ax2.hist(maxPowerRws[:,1], bins=n_bins, label='RW2')
ax2.legend()
ax3.hist(maxPowerRws[:,2], bins=n_bins, label='RW3')
ax3.legend()
ax4.hist(maxPowerRws[:,3], bins=n_bins, label='RW4')
ax4.legend()

ax1.set_xlabel('Peak Power [W]')
ax2.set_xlabel('Peak Power [W]')
ax3.set_xlabel('Peak Power [W]')
ax4.set_xlabel('Peak Power [W]')

ax1.set_ylabel('Count')
ax2.set_ylabel('Count')
ax3.set_ylabel('Count')
ax4.set_ylabel('Count')

plt.tight_layout()
plt.close()
if case_2_inertia == True: 
    fig.savefig(saveFigPath + 'case2_rw_power_hist.eps')
    pass 

# Reaction wheel peak power w.r.t to moments of inertia.

# left, width = 0.1, 0.65
# bottom, height = 0.1, 0.65
# bottom_h = left_h = left + width + 0.02

# rect_scatter = [left, bottom, width, height]
# rect_histx = [left, bottom_h, width, 0.2]
# rect_histy = [left_h, bottom, 0.2, height]

# plt.figure(1, figsize=(8, 8))
# axScatter = plt.axes(rect_scatter)
# axHistx = plt.axes(rect_histx)
# axHisty = plt.axes(rect_histy)
# axScatter.scatter( momentsOfInertia[0], maxPowerRws[:,0], color='r', marker='o', label='RW1')
# axHistx.hist(momentsOfInertia[0])
# axHisty.hist(maxPowerRws[:,0], orientation='horizontal')
# fig     = plt.figure(figsize=figureSize)
# ax1     = fig.add_subplot(2,2,1)
# ax2     = fig.add_subplot(2,2,2)
# ax3     = fig.add_subplot(2,2,3)
# ax4     = fig.add_subplot(2,2,4)

# ax1.scatter( momentsOfInertia[0], maxPowerRws[:,0], color='r', marker='o', label='RW1')
# ax1.legend()
# ax2.scatter( momentsOfInertia[0], maxPowerRws[:,1], color='b', marker='o', label='RW2')
# ax2.legend()
# ax3.scatter( momentsOfInertia[0], maxPowerRws[:,2], color='g', marker='o', label='RW3')
# ax3.legend()
# ax4.scatter( momentsOfInertia[0], maxPowerRws[:,3], color='c', marker='o', label='RW4')
# ax4.legend()

# ax1.set_xlabel('J_xx [kg/m2]')
# ax2.set_xlabel('J_xx [kg/m2]')
# ax3.set_xlabel('J_xx [kg/m2]')
# ax4.set_xlabel('J_xx [kg/m2]')

# ax1.set_ylabel('Peak power [W]')
# ax2.set_ylabel('Peak power [W]')
# ax3.set_ylabel('Peak power [W]')
# ax4.set_ylabel('Peak power [W]')

# plt.close()

if case_2_inertia == True: 
    fig.savefig(saveFigPath + 'case2_rw_power_vs_moi.eps')
    pass 

# Histogram of peak momentum 
# fig     = plt.figure(figsize=figureSize)
# ax1     = fig.add_subplot(2,2,1)
# ax2     = fig.add_subplot(2,2,2)
# ax3     = fig.add_subplot(2,2,3)
# ax4     = fig.add_subplot(2,2,4)

# ax1.hist(maxMomentum[:,0], bins=n_bins, label='RW1')
# ax1.legend()
# ax2.hist(maxMomentum[:,1], bins=n_bins, label='RW2')
# ax2.legend()
# ax3.hist(maxMomentum[:,2], bins=n_bins, label='RW3')
# ax3.legend()
# ax4.hist(maxMomentum[:,3], bins=n_bins, label='RW4')
# ax4.legend()

# ax1.set_xlabel('Peak momentum [Nms]')
# ax2.set_xlabel('Peak momentum [Nms]')
# ax3.set_xlabel('Peak momentum [Nms]')
# ax4.set_xlabel('Peak momentum [Nms]')

# ax1.set_ylabel('Count')
# ax2.set_ylabel('Count')
# ax3.set_ylabel('Count')
# ax4.set_ylabel('Count')

# left, width = 0.1, 0.65
# bottom, height = 0.1, 0.65
# bottom_h = left_h = left + width + 0.02

# rect_scatter = [left, bottom, width, height]
# rect_histx = [left, bottom_h, width, 0.2]
# rect_histy = [left_h, bottom, 0.2, height]

# plt.figure(1, figsize=(8, 8))
# axScatter = plt.axes(rect_scatter)
# axHistx = plt.axes(rect_histx)
# axHisty = plt.axes(rect_histy)
# axScatter.scatter( momentsOfInertia[0], maxMomentum[:,0], color='r', marker='o', label='RW1')
# axHistx.hist(momentsOfInertia[0])
# axHisty.hist(maxMomentum[:,0], orientation='horizontal')

# axScatter.set_xlabel('MOIx [kg/m2]')
# axScatter.set_ylabel('Peak Momentum [Nms]')

# plt.tight_layout()
plt.close(False)
if case_2_inertia == True: 
    fig.savefig(saveFigPath + 'case2_rw_momentum_hist.eps')
    pass 

# Maximum momentum w.r.t moments of inertia. 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.scatter( momentsOfInertia[0], maxMomentum[:,0], label='RW1')
ax1.legend()
ax2.scatter( momentsOfInertia[0], maxMomentum[:,1], label='RW2')
ax2.legend()
ax3.scatter( momentsOfInertia[0], maxMomentum[:,2], label='RW3')
ax3.legend()
ax4.scatter( momentsOfInertia[0], maxMomentum[:,3], label='RW4')
ax4.legend()

ax1.set_xlabel('J_xx [kg/m2]')
ax2.set_xlabel('J_xx [kg/m2]')
ax3.set_xlabel('J_xx [kg/m2]')
ax4.set_xlabel('J_xx [kg/m2]')

ax1.set_ylabel('Peak momentum [Nms]')
ax2.set_ylabel('Peak momentum [Nms]')
ax3.set_ylabel('Peak momentum [Nms]')
ax4.set_ylabel('Peak momentum [Nms]')

if case_2_inertia == True: 
    fig.savefig(saveFigPath + 'case2_moi_vs_peak_momentum.eps')
    pass 
plt.close()

# Histogram of avg momentum 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.hist(avgMomentum[:,0], bins=n_bins, label='RW1')
ax1.legend()
ax2.hist(avgMomentum[:,1], bins=n_bins, label='RW2')
ax2.legend()
ax3.hist(avgMomentum[:,2], bins=n_bins, label='RW3')
ax3.legend()
ax4.hist(avgMomentum[:,3], bins=n_bins, label='RW4')
ax4.legend()

ax1.set_xlabel('Avg momentum [Nms]')
ax2.set_xlabel('Avg momentum [Nms]')
ax3.set_xlabel('Avg momentum [Nms]')
ax4.set_xlabel('Avg momentum [Nms]')

ax1.set_ylabel('Count')
ax2.set_ylabel('Count')
ax3.set_ylabel('Count')
ax4.set_ylabel('Count')

plt.tight_layout()
plt.close()
if case_2_inertia == True: 
    fig.savefig(saveFigPath + 'case2_rw_avg_momentum_hist.eps')
    pass 

# Average momentum w.r.t moments of inertia. 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.scatter( momentsOfInertia[0], avgMomentum[:,0], label='RW1')
ax1.legend()
ax2.scatter( momentsOfInertia[0], avgMomentum[:,1], label='RW2')
ax2.legend()
ax3.scatter( momentsOfInertia[0], avgMomentum[:,2], label='RW3')
ax3.legend()
ax4.scatter( momentsOfInertia[0], avgMomentum[:,3], label='RW4')
ax4.legend()

ax1.set_xlabel('J_xx [kg/m2]')
ax2.set_xlabel('J_xx [kg/m2]')
ax3.set_xlabel('J_xx [kg/m2]')
ax4.set_xlabel('J_xx [kg/m2]')

ax1.set_ylabel('Avg momentum [Nms]')
ax2.set_ylabel('Avg momentum [Nms]')
ax3.set_ylabel('Avg momentum [Nms]')
ax4.set_ylabel('Avg momentum [Nms]')
if case_2_inertia == True: 
    fig.savefig(saveFigPath + 'case2_moi_vs_avg_momentum.eps')
    pass 
plt.close()

# Angular velocity maximum w.r.t moments of inertia. 
fig      = plt.figure(figsize=figureSize)
ax1      = plt.subplot(411)
ax2      = plt.subplot(412)
ax3      = plt.subplot(413)
ax4      = plt.subplot(414)

ax1.scatter( momentsOfInertia[0], maxVelocity[:,0] )
ax2.scatter( momentsOfInertia[0], maxVelocity[:,1] )
ax3.scatter( momentsOfInertia[0], maxVelocity[:,2] )
ax4.scatter( momentsOfInertia[0], maxVelocity[:,3] )

ax1.set_xlabel('I_xx [kg/m2]')
ax1.set_title('Maximum Reaction wheel velocity [rpm]')

plt.close()

# Angular velocity avg w.r.t moments of inertia. 
fig      = plt.figure(figsize=figureSize)
ax1      = plt.subplot(411)
ax2      = plt.subplot(412)
ax3      = plt.subplot(413)
ax4      = plt.subplot(414)

ax1.scatter( momentsOfInertia[0], avgVelocity[:,0] )
ax2.scatter( momentsOfInertia[0], avgVelocity[:,1] )
ax3.scatter( momentsOfInertia[0], avgVelocity[:,2] )
ax4.scatter( momentsOfInertia[0], avgVelocity[:,3] )

ax1.set_xlabel('I_xx [kg/m2]')
ax1.set_title('Avg Reaction wheel velocity [rpm]')

plt.close()

# ######################################################################################### 
############################### Slew rate Monte carlo #####################################
# ######################################################################################### 

# Case 2: Plot histograms for reaction wheel peak power. 
fig     = plt.figure( figsize=figureSize )
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.hist(maxPowerRws[:,0], bins=n_bins, label='RW1')
ax1.legend()
ax2.hist(maxPowerRws[:,1], bins=n_bins, label='RW2')
ax2.legend()
ax3.hist(maxPowerRws[:,2], bins=n_bins, label='RW3')
ax3.legend()
ax4.hist(maxPowerRws[:,3], bins=n_bins, label='RW4')
ax4.legend()

ax1.set_xlabel('Peak Power [W]')
ax2.set_xlabel('Peak Power [W]')
ax3.set_xlabel('Peak Power [W]')
ax4.set_xlabel('Peak Power [W]')

ax1.set_ylabel('Count')
ax2.set_ylabel('Count')
ax3.set_ylabel('Count')
ax4.set_ylabel('Count')

plt.tight_layout()
plt.close()
if case_2_slewRate == True: 
    fig.savefig(saveFigPath + 'case2_slewRate_rw_power_hist.eps')
    pass 

# Reaction wheel peak power w.r.t to moments of inertia. 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.scatter( slewRate, maxPowerRws[:,0], color='r', marker='o', label='RW1')
ax1.legend()
ax2.scatter( slewRate, maxPowerRws[:,1], color='b', marker='o', label='RW2')
ax2.legend()
ax3.scatter( slewRate, maxPowerRws[:,2], color='g', marker='o', label='RW3')
ax3.legend()
ax4.scatter( slewRate, maxPowerRws[:,3], color='c', marker='o', label='RW4')
ax4.legend()

ax1.set_xlabel('Slew rate [deg/sec]')
ax2.set_xlabel('Slew rate [deg/sec]')
ax3.set_xlabel('Slew rate [deg/sec]')
ax4.set_xlabel('Slew rate [deg/sec]')

ax1.set_ylabel('Peak power [W]')
ax2.set_ylabel('Peak power [W]')
ax3.set_ylabel('Peak power [W]')
ax4.set_ylabel('Peak power [W]')

plt.close()

if case_2_slewRate == True: 
    fig.savefig(saveFigPath + 'case2_slewRate_rw_power_vs_moi.eps')
    pass 

# Histogram of peak momentum 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.hist(maxMomentum[:,0], bins=n_bins, label='RW1')
ax1.legend()
ax2.hist(maxMomentum[:,1], bins=n_bins, label='RW2')
ax2.legend()
ax3.hist(maxMomentum[:,2], bins=n_bins, label='RW3')
ax3.legend()
ax4.hist(maxMomentum[:,3], bins=n_bins, label='RW4')
ax4.legend()

ax1.set_xlabel('Peak momentum [Nms]')
ax2.set_xlabel('Peak momentum [Nms]')
ax3.set_xlabel('Peak momentum [Nms]')
ax4.set_xlabel('Peak momentum [Nms]')

ax1.set_ylabel('Count')
ax2.set_ylabel('Count')
ax3.set_ylabel('Count')
ax4.set_ylabel('Count')

plt.tight_layout()
plt.close()
if case_2_slewRate == True: 
    fig.savefig(saveFigPath + 'case2_slewRate_rw_momentum_hist.eps')
    pass 

# Maximum momentum w.r.t moments of inertia. 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.scatter( slewRate, maxMomentum[:,0], label='RW1')
ax1.legend()
ax2.scatter( slewRate, maxMomentum[:,1], label='RW2')
ax2.legend()
ax3.scatter( slewRate, maxMomentum[:,2], label='RW3')
ax3.legend()
ax4.scatter( slewRate, maxMomentum[:,3], label='RW4')
ax4.legend()

ax1.set_xlabel('Slew rate [deg/sec]')
ax2.set_xlabel('Slew rate [deg/sec]')
ax3.set_xlabel('Slew rate [deg/sec]')
ax4.set_xlabel('Slew rate [deg/sec]')

ax1.set_ylabel('Peak momentum [Nms]')
ax2.set_ylabel('Peak momentum [Nms]')
ax3.set_ylabel('Peak momentum [Nms]')
ax4.set_ylabel('Peak momentum [Nms]')

if case_2_slewRate == True: 
    fig.savefig(saveFigPath + 'case2_slewRate_moi_vs_peak_momentum.eps')
    pass 
plt.close()

# Histogram of avg momentum 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.hist(avgMomentum[:,0], bins=n_bins, label='RW1')
ax1.legend()
ax2.hist(avgMomentum[:,1], bins=n_bins, label='RW2')
ax2.legend()
ax3.hist(avgMomentum[:,2], bins=n_bins, label='RW3')
ax3.legend()
ax4.hist(avgMomentum[:,3], bins=n_bins, label='RW4')
ax4.legend()

ax1.set_xlabel('Avg momentum [Nms]')
ax2.set_xlabel('Avg momentum [Nms]')
ax3.set_xlabel('Avg momentum [Nms]')
ax4.set_xlabel('Avg momentum [Nms]')

ax1.set_ylabel('Count')
ax2.set_ylabel('Count')
ax3.set_ylabel('Count')
ax4.set_ylabel('Count')

plt.tight_layout()
plt.close()
if case_2_slewRate == True: 
    fig.savefig(saveFigPath + 'case2_slewRate_rw_avg_momentum_hist.eps')
    pass 

# Average momentum w.r.t moments of inertia. 
fig     = plt.figure(figsize=figureSize)
ax1     = fig.add_subplot(2,2,1)
ax2     = fig.add_subplot(2,2,2)
ax3     = fig.add_subplot(2,2,3)
ax4     = fig.add_subplot(2,2,4)

ax1.scatter( slewRate, avgMomentum[:,0], label='RW1')
ax1.legend()
ax2.scatter( slewRate, avgMomentum[:,1], label='RW2')
ax2.legend()
ax3.scatter( slewRate, avgMomentum[:,2], label='RW3')
ax3.legend()
ax4.scatter( slewRate, avgMomentum[:,3], label='RW4')
ax4.legend()

ax1.set_xlabel('Slew rate [deg/sec]')
ax2.set_xlabel('Slew rate [deg/sec]')
ax3.set_xlabel('Slew rate [deg/sec]')
ax4.set_xlabel('Slew rate [deg/sec]')

ax1.set_ylabel('Avg momentum [Nms]')
ax2.set_ylabel('Avg momentum [Nms]')
ax3.set_ylabel('Avg momentum [Nms]')
ax4.set_ylabel('Avg momentum [Nms]')
if case_2_slewRate == True: 
    fig.savefig(saveFigPath + 'case2_slewRate_moi_vs_avg_momentum.eps')
    pass 
plt.close()

# Angular velocity maximum w.r.t moments of inertia. 
fig      = plt.figure(figsize=figureSize)
ax1      = plt.subplot(411)
ax2      = plt.subplot(412)
ax3      = plt.subplot(413)
ax4      = plt.subplot(414)

ax1.scatter( slewRate, maxVelocity[:,0] )
ax2.scatter( slewRate, maxVelocity[:,1] )
ax3.scatter( slewRate, maxVelocity[:,2] )
ax4.scatter( slewRate, maxVelocity[:,3] )

ax1.set_xlabel('Slew Rate [deg/sec]')
ax1.set_title('Maximum Reaction wheel velocity [rpm]')

plt.close()

# Angular velocity avg w.r.t moments of inertia. 
fig      = plt.figure(figsize=figureSize)
ax1      = plt.subplot(411)
ax2      = plt.subplot(412)
ax3      = plt.subplot(413)
ax4      = plt.subplot(414)

ax1.scatter( slewRate, avgVelocity[:,0] )
ax2.scatter( slewRate, avgVelocity[:,1] )
ax3.scatter( slewRate, avgVelocity[:,2] )
ax4.scatter( slewRate, avgVelocity[:,3] )

ax1.set_xlabel('Slew Rate [deg/sec]')
ax1.set_title('Avg Reaction wheel velocity [rpm]')

plt.close()

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