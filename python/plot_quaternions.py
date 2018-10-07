'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''
# def requiredFiles( pathToDirectory, stringOfInterestingFiles, *moreKeywords ):
    
#     if not moreKeywords: 
#         interestingFiles    = [pathToDirectory + '/' + filename for filename in os.listdir(pathToDirectory) if os.path.isfile(pathToDirectory + '/' + filename) and stringOfInterestingFiles in filename]        
#     elif len(moreKeywords) == 1: 
#         interestingFiles    = [pathToDirectory + '/' + filename for filename in os.listdir(pathToDirectory) if os.path.isfile(pathToDirectory + '/' + filename) and stringOfInterestingFiles in filename and moreKeywords[0] in filename]
#         pass 
#     return interestingFiles

# Set up modules and packages.
# Plotting
import matplotlib
# matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import matplotlib.lines as mlines
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

# Post processign functions 
from functions import calculatePower
from functions import calculatePeakPowerPercentage
from functions import calculateSaturationPercentage
from functions import calculateSettlingTime

# plt.style.use('seaborn-whitegrid')
plt.style.use('dark_background')
# plt.style.use(['dark_background', 'presentation'])
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
# path                =  "/home/superman/Desktop/monte_carlo_single/"
path                        = "/home/superman/Desktop/single_simulation/"
metadataFilePath            = "/home/superman/Desktop/single_simulation/metadata.csv"
# metadata                    = pd.read_csv(metadataFilePath)
stringToSearchWith          = 'state_history.csv'
# numberOfReactionWheels      = '_44.csv'
# filesForTheplots            = requiredFiles( path, stringToSearchWith, numberOfReactionWheels)
filesForTheplots            = requiredFiles( path, stringToSearchWith )

# Define linestyles. 
linestyles = ['-', '--', '-.', ':']

# Define colors 
colors  = ['r', 'b', 'c', 'g']

# Change the font size 
font = {'family' : 'monospace',
        'weight' : 'bold',
        'size'   :  22}
matplotlib.rc('font', **font)        
matplotlib.rc('xtick', labelsize=22)
matplotlib.rc('ytick', labelsize=22)

closeplot    = True 
# savefigPath  = '/home/superman/Delft/Thesis/IAC/iac_2018/Images/results/case1/'
savefigPath    = '/home/superman/Delft/IAC/presentation/results/case1/'
figureSize = (14.0,6.0)
## Plot spacecraft attitudes. ## 
fig1  = plt.figure(figsize=figureSize)
ax1   = fig1.add_subplot(111)

for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )

    # Plot quaternion 1 with time.
    # ax1.plot(state_history['t']/60.0,state_history['q1'], c='r', linestyle='-', linewidth=5.0)
    # ax1.plot(state_history['t']/60.0,state_history['q2'], c='b', linestyle='--', linewidth=5.0)
    # ax1.plot(state_history['t']/60.0,state_history['q3'], c='c', linestyle='-.', linewidth=5.0)
    # ax1.plot(state_history['t']/60.0,state_history['q4'], c='g', linestyle=':', linewidth=5.0)

    
    ax1.plot(state_history['t']/60.0,state_history['q1'], c='r', linestyle='-', linewidth=2.0)
    ax1.plot(state_history['t']/60.0,state_history['q2'], c='b', linestyle='--', linewidth=2.0)
    ax1.plot(state_history['t']/60.0,state_history['q3'], c='c', linestyle='-.', linewidth=2.0)
    ax1.plot(state_history['t']/60.0,state_history['q4'], c='g', linestyle=':', linewidth=2.0)

    ax1.set_ylabel('Quaternions [-]')
    ax1.set_xlabel('Time [min]')

    ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax1.grid(linestyle='--', linewidth=0.25, color='white')

    plt.tight_layout()
    q1 = mlines.Line2D([], [], color='r', linestyle='-',
                          markersize=10, label='q1')
    q2 = mlines.Line2D([], [], color='b', linestyle='--',
                          markersize=10, label='q2')
    q3 = mlines.Line2D([], [], color='c', linestyle='-.',
                          markersize=10, label='q3')
    q4 = mlines.Line2D([], [], color='g', linestyle=':',
                          markersize=10, label='q4') 
    # plt.legend(handles=[q1,q2,q3,q4],loc=2) 
    # plt.legend(handles=[q1,q2,q3,q4]) 
    # plt.grid()
    pass

fig1.savefig(savefigPath + 'attitudes.png')
plt.close()

## PLot spacecraft control torques ##
fig  = plt.figure()
ax1   = fig.add_subplot(111)
# # fig, ( ax1, ax2, ax3, ax4 ) = plt.subplots(4, sharex=True, figsize=(11,7) )
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    #Generate figure with 2D plots. 
    # ax1 = fig.add_subplot(2,2,1)
    # ax2 = fig.add_subplot(2,2,2)
    # ax3 = fig.add_subplot(2,2,3)

    #Plot angular velocity with respect to time. 
    ax1.plot(state_history['t']/60.0, state_history['controlTorque1'], c=colors[0], linestyle=linestyles[0] )
    ax1.plot(state_history['t']/60.0, state_history['controlTorque2'], c=colors[1], linestyle=linestyles[1])
    ax1.plot(state_history['t']/60.0, state_history['controlTorque3'], c=colors[2], linestyle=linestyles[2])
    ax1.grid()

    ax1.set_xlabel('Time[min]')
    ax1.set_ylabel('Control Torque [N m]')
    ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    

    # ax2.plot(state_history['t'], state_history['controlTorque2'])
    # ax2.set_xlabel('time[sec]')
    # ax2.set_ylabel('Control_Y [Nm]')
    # ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax2.grid()

    # ax3.plot(state_history['t'], state_history['controlTorque3'])
    # ax3.set_xlabel('time[sec]')
    # ax3.set_ylabel('Control_Z [Nm]')
    # ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax3.grid()    
    plt.tight_layout()
    
    header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                          markersize=10, label='Control Torque in X axis')
    header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                          markersize=10, label='Control Torque in Y axis')
    header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                          markersize=10, label='Control Torque in Z axis')
    plt.legend(handles=[header1,header2,header3])
    plt.grid()

    pass

fig.savefig(savefigPath +'controlTorques.eps')
plt.close(closeplot)

## Plot motor torques of the reaction wheels ##
fig = plt.figure()
ax  = fig.add_subplot(111)

for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'motorTorque'
    results = stringLocator(testPath, stringToBeLocated )

    jj = np.ceil( len(results)/ 2.0 )
    for i in range(len(results)):
        # ax = fig.add_subplot(2,jj,i+1)
        ax.plot( state_history['t']/60.0, state_history[results[i]], color=colors[i], linestyle=linestyles[i], linewidth=2.0 )
        ax.set_xlabel('Time [min]')
        ax.set_ylabel('Motor torque [N/m]')
        ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
        pass 
    ax.grid()

    plt.tight_layout()
    
    header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                          markersize=10, label='RW1 motor torque')
    header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                          markersize=10, label='RW2 motor torque')
    header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                          markersize=10, label='RW3 motor torque')
    header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                          markersize=10, label='RW4 motor torque')   
    plt.legend(handles=[header1,header2,header3,header4])

    plt.grid()
    
    pass 
fig.savefig(savefigPath +'motorTorques.eps')
plt.close(closeplot)

# ## Plot angular momentum of the reaction wheels. 
fig = plt.figure()
ax  = fig.add_subplot(111)

for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'angularMomentum'
    results = stringLocator(testPath, stringToBeLocated )

    for i in range(len(results)):
        jj = np.ceil( len(results)/ 2.0 )
        # ax = fig.add_subplot(2,jj,i+1)
        ax.plot( state_history['t']/60.0, state_history[results[i]], color=colors[i], linestyle=linestyles[i] )
        ax.set_xlabel('Time[min]')
        ax.set_ylabel('Angular momentum [kgm2/sec]')
        ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
        pass 
    
        ax.grid()
        plt.tight_layout()
        header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                              markersize=10, label='RW1 angular momentum')
        header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                              markersize=10, label='RW2 angular momentum')
        header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                              markersize=10, label='RW3 angular momentum')
        header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                              markersize=10, label='RW4 angular momentum')   
        plt.legend(handles=[header1,header2,header3,header4])

        plt.grid()
    pass 

fig.savefig(savefigPath +'angularMomentumReactionWheels.eps')
plt.close(closeplot)

## Plot the reaction wheel angular velocities. 
fig = plt.figure()
ax = fig.add_subplot(111)

for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'reactionWheelAngularVelocity'
    results = stringLocator(testPath, stringToBeLocated )

    for i in range(len(results)):
        jj = np.ceil( len(results)/ 2.0 )
        ax.plot( state_history['t']/60.0, state_history[results[i]], color=colors[i], linestyle=linestyles[i] )
        ax.set_xlabel('Time[min]')
        ax.set_title('Angular Velocity [rpm]')
        ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    
    ax.grid()
    plt.tight_layout()
    header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                          markersize=10, label='RW1 angular velocity')
    header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                          markersize=10, label='RW2 angular velocity')
    header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                          markersize=10, label='RW3 angular velocity')
    header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                          markersize=10, label='RW4 angular velocity')   
    plt.legend(handles=[header1,header2,header3,header4])
    plt.grid()
    pass 
fig.savefig(savefigPath +'reactionWheelAngularRates.eps')
plt.close(closeplot)

## Plot the power consumption for the reaction wheels. ##  
fig = plt.figure()
ax = fig.add_subplot(111)
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'powerConsumption'
    results = stringLocator(testPath, stringToBeLocated )
    jj = np.ceil( len(results)/ 2.0 )
    
    for i in range(len(results)):
        ax.plot( state_history['t']/60.0, state_history[results[i]], color=colors[i], linestyle=linestyles[i] )
        ax.set_ylabel( 'Power [W]' )
        ax.set_xlabel('Time[min]')
        # ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    ax.grid()
    plt.tight_layout()
    header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                          markersize=10, label='RW1 power consumption')
    header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                          markersize=10, label='RW2 power consumption')
    header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                          markersize=10, label='RW3 power consumption')
    header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                          markersize=10, label='RW4 power consumption')   
    plt.legend(handles=[header1,header2,header3,header4])
    plt.grid()
    
    pass 
fig.savefig(savefigPath +'power.eps')
plt.close(closeplot)

## Plot the angular rates of the spacecraft ## 
fig = plt.figure()
# fig, ( ax1, ax2, ax3, ax4 ) = plt.subplots(4, sharex=True, figsize=(11,7) )
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    ax1 = fig.add_subplot(2,2,1)
    ax2 = fig.add_subplot(2,2,2)
    ax3 = fig.add_subplot(2,2,3)
    ax4 = fig.add_subplot(2,2,4)

    #Plot angular velocity with respect to time. 
    ax1.plot(state_history['t'] / 60.0, state_history['w1'])
    # ax1.set_xlabel('time[sec]')
    ax1.set_ylabel('V_x[deg/sec]')
    ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax1.grid()

    ax2.plot(state_history['t'] / 60.0, state_history['w2'])
    # ax2.set_xlabel('time[sec]')
    ax2.set_ylabel('V_y [deg/sec]')
    ax2.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax2.grid()

    ax3.plot(state_history['t'] / 60.0, state_history['w3'])
    ax3.set_xlabel('Time[min]')
    ax3.set_ylabel('V_z [deg/sec]')
    ax3.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax3.grid()

    ax4.plot(state_history['t'] / 60.0, state_history['slewRate'])
    ax4.set_xlabel('Time[min]')
    ax4.set_ylabel('Slew Rate [deg/sec]')
    ax4.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax4.grid()

    plt.tight_layout()
    
    plt.grid()
    
    pass 
fig.savefig(savefigPath +'angularRatesSpacecraft.eps')
plt.close(closeplot)

plt.show()

print "Figures generated successfully!"
print ""

peakPowerPercents   = calculatePeakPowerPercentage( metadataFilePath, filesForTheplots )
peakMomentumPercent = calculateSaturationPercentage( metadataFilePath, filesForTheplots )
settlingTime        = calculateSettlingTime("/home/superman/Desktop/single_simulation/state_history.csv")
maxPower, yy, xx, zz            = calculatePower(filesForTheplots)

print("Peak power %:", peakPowerPercents)
print("Peak momentum %:", peakMomentumPercent)
print("Settling Time [min]: ", (settlingTime) )
print("Total peak power [W]: ", maxPower)
# Stop timer
end_time = time.time( )

print ""
print "------------------------------------------------------------------"
print "                         Exited successfully!                     "
print "------------------------------------------------------------------"
print ""