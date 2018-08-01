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
# path                =  "/home/superman/Desktop/single_simulation/"
path                        = "/home/superman/Desktop/single_simulation/"
stringToSearchWith          = 'state_history'
numberOfReactionWheels      = '_44.csv'
# filesForTheplots            = requiredFiles( path, stringToSearchWith, numberOfReactionWheels)
filesForTheplots            = requiredFiles( path, stringToSearchWith )

## Plot spacecraft attitudes. ## 
fig, ( ax1, ax2, ax3, ax4 ) = plt.subplots(4, sharex=False, figsize=(11,7) )
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )

    # Plot quaternion 1 with time.
    ax1.plot(state_history['t']/60.0,state_history['q1'])
    ax1.set_ylabel('q_1')
    ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax1.grid()
    
    # Plot quaternion 1 with time.
    ax2.plot(state_history['t']/60.0,state_history['q2'])
    ax2.set_ylabel('q_2')
    ax2.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax2.grid()

    # # Plot quaternion 1 with time.
    ax3.plot(state_history['t']/60.0,state_history['q3'])
    ax3.set_ylabel('q_3')
    ax3.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax3.grid()

    # # Plot quaternion 1 with time.
    ax4.plot(state_history['t'] / 60.0,state_history['q4'])
    ax4.set_xlabel('Time [min]')
    ax4.set_ylabel('q_4')
    ax4.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax4.grid()

    plt.tight_layout()

    pass
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/attitudes.eps')
# # plt.close()

## PLot spacecraft control torques ##
fig = plt.figure()
# fig, ( ax1, ax2, ax3, ax4 ) = plt.subplots(4, sharex=True, figsize=(11,7) )
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    #Generate figure with 2D plots. 
    ax1 = fig.add_subplot(2,2,1)
    ax2 = fig.add_subplot(2,2,2)
    ax3 = fig.add_subplot(2,2,3)

    #Plot angular velocity with respect to time. 
    ax1.plot(state_history['t'], state_history['controlTorque1'])
    ax1.set_xlabel('time[sec]')
    ax1.set_ylabel('Control_X [Nm]')
    ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax1.grid()

    ax2.plot(state_history['t'], state_history['controlTorque2'])
    ax2.set_xlabel('time[sec]')
    ax2.set_ylabel('Control_Y [Nm]')
    ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax2.grid()

    ax3.plot(state_history['t'], state_history['controlTorque3'])
    ax3.set_xlabel('time[sec]')
    ax3.set_ylabel('Control_Z [Nm]')
    ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax3.grid()

    plt.tight_layout()
    # plt.grid()

    pass

# # fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/controlTorques.eps')
# plt.close()

## Plot motor torques of the reaction wheels ##
fig = plt.figure()
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'motorTorque'
    results = stringLocator(testPath, stringToBeLocated )

    jj = np.ceil( len(results)/ 2.0 )
    for i in range(len(results)):
        ax = fig.add_subplot(2,jj,i+1)
        ax.plot( state_history['t'], state_history[results[i]] )
        ax.set_xlabel('time[sec]')
        ax.set_ylabel('Motor torque [N/m]')
        ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    plt.tight_layout()

    pass 
# # fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/motorTorques.eps')
# plt.close()

## Plot angular momentum of the reaction wheels. 
fig = plt.figure()

for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'angularMomentum'
    results = stringLocator(testPath, stringToBeLocated )

    for i in range(len(results)):
        jj = np.ceil( len(results)/ 2.0 )
        ax = fig.add_subplot(2,jj,i+1)
        ax.plot( state_history['t'], state_history[results[i]] )
        ax.set_xlabel('time[sec]')
        ax.set_ylabel('Angular momentum [kgm2/sec]')
        ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    plt.tight_layout()
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/angularMomentumReactionWheels.eps')
plt.close()

## Plot the reaction wheel angular velocities. 
fig = plt.figure()
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'reactionWheelAngularVelocity'
    results = stringLocator(testPath, stringToBeLocated )

    for i in range(len(results)):
        jj = np.ceil( len(results)/ 2.0 )
        ax = fig.add_subplot(2,jj,i+1)
        ax.plot( state_history['t'], state_history[results[i]] )
        ax.set_xlabel('time[sec]')
        ax.set_title('Angular Velocity [rpm]')
        ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    plt.tight_layout()
    # plt.grid()
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/reactionWheelAngularRates.eps')
# plt.close()

## Plot the power consumption for the reaction wheels. ##  
fig = plt.figure(figsize=(11,7))
for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )
    testPath   = filesForTheplots[filename]
    stringToBeLocated   = 'powerConsumption'
    results = stringLocator(testPath, stringToBeLocated )
    jj = np.ceil( len(results)/ 2.0 )
    
    for i in range(len(results)):

        ax = fig.add_subplot(2,jj,i+1)
        ax.plot( state_history['t']/60.0, state_history[results[i]] )
        ax.set_title( 'Power' + str(i+1) + ' [W]' )
        ax.set_xlabel('time[min]')
        # ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    plt.tight_layout()
    plt.grid()
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/power.eps')
# plt.close()

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
    ax3.set_xlabel('time[min]')
    ax3.set_ylabel('V_z [deg/sec]')
    ax3.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax3.grid()

    ax4.plot(state_history['t'] / 60.0, state_history['slewRate'])
    ax4.set_xlabel('time[min]')
    ax4.set_ylabel('Slew Rate [deg/sec]')
    ax4.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax4.grid()

    plt.tight_layout()
    # plt.grid()
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/angularRatesSpacecraft.eps')
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