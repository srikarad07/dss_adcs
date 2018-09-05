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
# path                =  "/home/superman/Desktop/monte_carlo_single/"
path                        = "/home/superman/Desktop/single_simulation/"
stringToSearchWith          = 'state_history.csv'
numberOfReactionWheels      = '_44.csv'
# filesForTheplots            = requiredFiles( path, stringToSearchWith, numberOfReactionWheels)
filesForTheplots            = requiredFiles( path, stringToSearchWith )

# Define linestyles. 
linestyles = ['-', '--', '-.', ':']

# Define colors 
colors  = ['r', 'b', 'c', 'g']

## Plot spacecraft attitudes. ## 
fig1  = plt.figure()
ax1   = fig1.add_subplot(111)

for filename in range(len(filesForTheplots)):

    state_history   = pd.read_csv( filesForTheplots[filename] )

    # Plot quaternion 1 with time.
    ax1.plot(state_history['t']/60.0,state_history['q1'], c='r', linestyle='-')
    ax1.plot(state_history['t']/60.0,state_history['q2'], c='b', linestyle='--')
    ax1.plot(state_history['t']/60.0,state_history['q3'], c='g', linestyle='-.')
    ax1.plot(state_history['t']/60.0,state_history['q4'], c='y', linestyle=':')

    ax1.set_ylabel('quaternions')
    ax1.set_xlabel('Time [min]')

    ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    ax1.grid()
    
    # Plot quaternion 1 with time.
    # ax2.set_ylabel('q_2')
    # ax2.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    # ax2.grid()

    # # # Plot quaternion 1 with time.
    # ax3.set_ylabel('q_3')
    # ax3.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    # ax3.grid()

    # # # Plot quaternion 1 with time.
    # 
    
    # ax4.set_ylabel('q_4')
    # ax4.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    # ax4.grid()

    plt.tight_layout()
    q1 = mlines.Line2D([], [], color='r', linestyle='-',
                          markersize=10, label='q1')
    q2 = mlines.Line2D([], [], color='b', linestyle='--',
                          markersize=10, label='q2')
    q3 = mlines.Line2D([], [], color='g', linestyle='-.',
                          markersize=10, label='q3')
    q4 = mlines.Line2D([], [], color='y', linestyle=':',
                          markersize=10, label='q4') 
    plt.legend(handles=[q1,q2,q3,q4]) 
    plt.grid()
    pass

# fig1.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/case1/attitudes.eps')
# plt.close()

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

    ax1.set_xlabel('time[min]')
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

# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/case1/controlTorques.eps')
# plt.close()

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
        ax.plot( state_history['t'], state_history[results[i]], color=colors[i], linestyle=linestyles[i] )
        ax.set_xlabel('time[sec]')
        ax.set_ylabel('Motor torque [N/m]')
        ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    ax.grid()

    plt.tight_layout()
    
    header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                          markersize=10, label='RW1 motor torque')
    header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                          markersize=10, label='RW1 motor torque')
    header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                          markersize=10, label='RW1 motor torque')
    header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                          markersize=10, label='RW1 motor torque')   
    plt.legend(handles=[header1,header2,header3,header4])

    plt.grid()
    
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/case1/motorTorques.eps')
# plt.close()

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
        ax.set_xlabel('time[min]')
        ax.set_ylabel('Angular momentum [kgm2/sec]')
        ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
        pass 
    
        ax.grid()
        plt.tight_layout()
        header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                              markersize=10, label='RW1 angular momentum')
        header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                              markersize=10, label='RW1 angular momentum')
        header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                              markersize=10, label='RW1 angular momentum')
        header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                              markersize=10, label='RW1 angular momentum')   
        plt.legend(handles=[header1,header2,header3,header4])

        plt.grid()
    pass 

# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/case1/angularMomentumReactionWheels.eps')
# plt.close()

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
        ax.set_xlabel('time[min]')
        ax.set_title('Angular Velocity [rpm]')
        ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    
    ax.grid()
    plt.tight_layout()
    header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                          markersize=10, label='RW1 angular velocity')
    header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                          markersize=10, label='RW1 angular velocity')
    header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                          markersize=10, label='RW1 angular velocity')
    header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                          markersize=10, label='RW1 angular velocity')   
    plt.legend(handles=[header1,header2,header3,header4])
    plt.grid()
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/case1/reactionWheelAngularRates.eps')
# plt.close()

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
        ax.set_title( 'Power [W]' )
        ax.set_xlabel('time[min]')
        # ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    ax.grid()
    plt.tight_layout()
    header1 = mlines.Line2D([], [], color=colors[0], linestyle=linestyles[0],
                          markersize=10, label='RW1 power consumption')
    header2 = mlines.Line2D([], [], color=colors[1], linestyle=linestyles[1],
                          markersize=10, label='RW1 power consumption')
    header3 = mlines.Line2D([], [], color=colors[2], linestyle=linestyles[2],
                          markersize=10, label='RW1 power consumption')
    header4 = mlines.Line2D([], [], color=colors[3], linestyle=linestyles[3],
                          markersize=10, label='RW1 power consumption')   
    plt.legend(handles=[header1,header2,header3,header4])
    plt.grid()
    
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/case1/power.eps')
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
    
    plt.grid()
    
    pass 
# fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/case1/angularRatesSpacecraft.eps')
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