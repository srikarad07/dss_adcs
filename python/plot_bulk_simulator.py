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
from test_csv import stringLocator

plt.style.use('seaborn-whitegrid')
# plt.style.use('classic')

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
# path            =  "/home/superman/Desktop/state_history.csv"
path                =  "/home/superman/Desktop"

## Plot spacecraft attitudes. ## 
# fig = plt.figure(figsize=(11,7))
# fig, ((ax1, ax2), (ax3, ax4) ) = plt.subplots(2, 2, sharex=True, sharey=True )
fig, ( ax1, ax2, ax3, ax4 ) = plt.subplots(4, sharex=True, figsize=(11,7) )
for filename in os.listdir(path):

    state_history   = pd.read_csv( path + '/' + filename )
    
    # Generate figure with 2D views.
    # ax1 = fig.add_subplot(411)
    # ax2 = fig.add_subplot(412)
    # ax3 = fig.add_subplot(413)
    # ax4 = fig.add_subplot(414, frameon=True)
    # ax1  = fig.add_axes([0.1, 0.5, 0.8, 0.4])
    # ax2  = fig.add_axes([0.1, 0.1, 0.8, 0.4])
    # ax3 = fig.add_axes(xticklabels=[])

    # Plot quaternion 1 with time.
    ax1.plot(state_history['t']/60.0,state_history['q1'])
    # ax1.scatter(0.0,0.0,s=100,marker='o',color='b')
    # ax1.set_xlabel('time [sec]')
    ax1.set_ylabel('q_1')
    ax1.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
    ax1.grid()
    
    # axarr[0].plot(state_history['t']/60.0,state_history['q1'])
    # axarr[1].plot(state_history['t']/60.0,state_history['q2'])    
    # axarr[2].plot(state_history['t']/60.0,state_history['q3'])
    # axarr[3].plot(state_history['t']/60.0,state_history['q4'])
    
    # Plot quaternion 1 with time.
    ax2.plot(state_history['t']/60.0,state_history['q2'])
    # ax1.scatter(0.0,0.0,s=100,marker='o',color='b')
    # ax2.set_xlabel('time [sec]')
    ax2.set_ylabel('q_2')
    ax2.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
    ax2.grid()

    # # Plot quaternion 1 with time.
    ax3.plot(state_history['t']/60.0,state_history['q3'])
    # ax3.scatter(0.0,0.0,s=100,marker='o',color='b')
    # ax3.set_xlabel('time [min]')
    ax3.set_ylabel('q_3')
    ax3.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
    ax3.grid()

    # # Plot quaternion 1 with time.
    ax4.plot(state_history['t'] / 60.0,state_history['q4'])
    # ax4.scatter(0.0,0.0,s=100,marker='o',color='b')
    ax4.set_xlabel('Time [min]')
    ax4.set_ylabel('q_4')
    ax4.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
    ax4.grid()

    # plt.legend('Concept1', 'Concept2', 'Concept3', 'Concept4')
    # plt.tight_layout()
    plt.grid()

    pass
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/attitudes.svg')
plt.show()
## PLot spacecraft control torques ##
fig = plt.figure()
for filename in os.listdir(path): 

    state_history   = pd.read_csv( path + '/' + filename )
    #Generate figure with 2D plots. 
    ax1 = fig.add_subplot(2,2,1)
    ax2 = fig.add_subplot(2,2,2)
    ax3 = fig.add_subplot(2,2,3)

    #Plot angular velocity with respect to time. 
    ax1.plot(state_history['t'], state_history['controlTorque1'])
    ax1.set_xlabel('time[sec]')
    ax1.set_ylabel('Control_X [N/m]')
    ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax1.grid()

    ax2.plot(state_history['t'], state_history['controlTorque2'])
    ax2.set_xlabel('time[sec]')
    ax2.set_ylabel('Control_Y [N/m]')
    ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax2.grid()

    ax3.plot(state_history['t'], state_history['controlTorque3'])
    ax3.set_xlabel('time[sec]')
    ax3.set_ylabel('Control_Z [N/m]')
    ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax3.grid()

    plt.tight_layout()
    # plt.grid()

    pass
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/controlTorques.svg')

## Plot motor torques of the reaction wheels ##
fig = plt.figure()
for filename in os.listdir(path):

    state_history   = pd.read_csv( path + '/' + filename )
    testPath   =  path + '/' + filename
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
    # plt.tight_layout()
    plt.grid()
    pass 
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/motorTorques.svg')

## Plot angular momentum of the reaction wheels. 
fig = plt.figure()
for filename in os.listdir(path):

    state_history   = pd.read_csv( path + '/' + filename )
    testPath   =  path + '/' + filename
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
    # plt.tight_layout()
    plt.grid()
    pass 
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/angularMomentumReactionWheels.svg')
    # Generate figure with 2D plots. 
    # ax1 = fig.add_subplot(2,2,1)
    # ax2 = fig.add_subplot(2,2,2)
    # ax3 = fig.add_subplot(2,2,3)
    # ax4 = fig.add_subplot(2,2,4)
    # ax1 = fig.add_subplot(2,3,1)
    # ax2 = fig.add_subplot(2,3,2)
    # ax3 = fig.add_subplot(2,3,3)
    # ax4 = fig.add_subplot(2,3,4)
    # ax5 = fig.add_subplot(2,3,5)
    # ax6 = fig.add_subplot(2,3,6)

    # Plot angular velocity with respect to time. 
    # ax1.plot(state_history['t'], state_history['angularMomentum1'])
    # ax1.set_xlabel('time[sec]')
    # ax1.set_ylabel('Angular momentum [kgm2/sec]')
    # ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax1.grid()

    # ax2.plot(state_history['t'], state_history['angularMomentum2'])
    # ax2.set_xlabel('time[sec]')
    # ax2.set_ylabel('Angular momentum [kgm2/sec]')
    # ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax2.grid()

    # ax3.plot(state_history['t'], state_history['angularMomentum3'])
    # ax3.set_xlabel('time[sec]')
    # ax3.set_ylabel('Angular momentum [kgm2/sec]')
    # ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax3.grid()

    # ax4.plot(state_history['t'], state_history['angularMomentum4'])
    # ax4.set_xlabel('time[sec]')
    # ax4.set_ylabel('Angular momentum [kgm2/sec]')
    # ax4.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax4.grid()

    # ax5.plot(state_history['t'], state_history['angularMomentum5'])
    # ax5.set_xlabel('time[sec]')
    # ax5.set_ylabel('Angular momentum [kgm2/sec]')
    # ax5.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax5.grid()

    # ax6.plot(state_history['t'], state_history['angularMomentum6'])
    # ax6.set_xlabel('time[sec]')
    # ax6.set_ylabel('Angular momentum [kgm2/sec]')
    # ax6.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    # ax6.grid()

    # plt.tight_layout()
    # plt.grid()

    # pass 

## Plot the reaction wheel angular velocities. 
fig = plt.figure()
for filename in os.listdir(path):

    state_history   = pd.read_csv( path + '/' + filename )
    testPath   =  path + '/' + filename
    stringToBeLocated   = 'reactionWheelAngularVelocity'
    results = stringLocator(testPath, stringToBeLocated )

    for i in range(len(results)):
        jj = np.ceil( len(results)/ 2.0 )
        ax = fig.add_subplot(2,jj,i+1)
        ax.plot( state_history['t'], state_history[results[i]] )
        ax.set_xlabel('time[sec]')
        ax.set_ylabel('Angular Velocity [rad/sec]')
        ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
        pass 
    # plt.tight_layout()
    plt.grid()
    pass 
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/reactionWheelAngularRates.svg')
# fig = plt.figure()
# for filename in os.listdir(path):

#     state_history   = pd.read_csv( path + '/' + filename )
#     ax1 = fig.add_subplot(2,2,1)
#     ax2 = fig.add_subplot(2,2,2)
#     ax3 = fig.add_subplot(2,2,3)
#     ax4 = fig.add_subplot(2,2,4)
#     # ax1 = fig.add_subplot(2,3,1)
#     # ax2 = fig.add_subplot(2,3,2)
#     # ax3 = fig.add_subplot(2,3,3)
#     # ax4 = fig.add_subplot(2,3,4)
#     # ax5 = fig.add_subplot(2,3,5)
#     # ax6 = fig.add_subplot(2,3,6)

#     #Plot angular velocity wle='sci', axis='both', scilimits=(0,0))
#     ax1.grid()

#     ax2.plot(state_history['t'], state_history['reactionWheelAngularVelocity2'])
#     ax2.set_xlabel('time[sec]')
#     ax2.set_ylabel('Angular Velocity [rad/sec]')
#     ax2.ticklabel_format(style='sci', axis='boith respect to time. 
#     ax1.plot(state_history['t'], state_history['reactionWheelAngularVelocity1'])
#     ax1.set_xlabel('time[sec]')
#     ax1.set_ylabel('Angular Velocity [rad/sec]')
#     ax1.ticklabel_format(styth', scilimits=(0,0))
#     ax2.grid()

#     ax3.plot(state_history['t'], state_history['reactionWheelAngularVelocity3'])
#     ax3.set_xlabel('time[sec]')
#     ax3.set_ylabel('Angular Velocity [rad/sec]')
#     ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     ax3.grid()

#     ax4.plot(state_history['t'], state_history['reactionWheelAngularVelocity4'])
#     ax4.set_xlabel('time[sec]')
#     ax4.set_ylabel('Angular Velocity [rad/sec]')
#     ax4.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     ax4.grid()

#     # ax5.plot(state_history['t'], state_history['reactionWheelAngularVelocity5'])
#     # ax5.set_xlabel('time[sec]')
#     # ax5.set_ylabel('Angular Velocity [rad/sec]')
#     # ax5.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     # ax5.grid()

#     # ax6.plot(state_history['t'], state_history['reactionWheelAngularVelocity6'])
#     # ax6.set_xlabel('time[sec]')
#     # ax6.set_ylabel('Angular Velocity [rad/sec]')
#     # ax6.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     # ax6.grid()

#     # plt.tight_layout()
#     plt.grid()
#     pass  

## Plot the power consumption for the reaction wheels. ##  
fig = plt.figure(figsize=(11,7))
for filename in os.listdir(path):

    state_history   = pd.read_csv( path + '/' + filename )
    testPath   =  path + '/' + filename
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
    # plt.tight_layout()
    plt.grid()
    pass 
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/power.svg')
# fig = plt.figure()
# for filename in os.listdir(path):
#     state_history   = pd.read_csv( path + '/' + filename )
#     ax1 = fig.add_subplot(2,2,1)
#     ax2 = fig.add_subplot(2,2,2)
#     ax3 = fig.add_subplot(2,2,3)
#     ax4 = fig.add_subplot(2,2,4)
#     # ax1 = fig.add_subplot(2,3,1)
#     # ax2 = fig.add_subplot(2,3,2)
#     # ax3 = fig.add_subplot(2,3,3)
#     # ax4 = fig.add_subplot(2,3,4)
#     # ax5 = fig.add_subplot(2,3,5)
#     # ax6 = fig.add_subplot(2,3,6)

    
#     #Plot angular velocity with respect to time. 
#     ax1.plot(state_history['t'], state_history['powerConsumption1'])
#     ax1.set_xlabel('time[sec]')
#     ax1.set_ylabel('Power Consumption [W]')
#     ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     ax1.grid()

#     ax2.plot(state_history['t'], state_history['powerConsumption2'])
#     ax2.set_xlabel('time[sec]')
#     ax2.set_ylabel('Power Consumption [W]')
#     ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     ax2.grid()

#     ax3.plot(state_history['t'], state_history['powerConsumption3'])
#     ax3.set_xlabel('time[sec]')
#     ax3.set_ylabel('Power Consumption [W]')
#     ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     ax3.grid()

#     ax4.plot(state_history['t'], state_history['powerConsumption4'])
#     ax4.set_xlabel('time[sec]')
#     ax4.set_ylabel('Power Consumption [W]')
#     ax4.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     ax4.grid()

#     # ax5.plot(state_history['t'], state_history['powerConsumption5'])
#     # ax5.set_xlabel('time[sec]')
#     # ax5.set_ylabel('Power Consumption [W]')
#     # ax5.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     # ax5.grid()

#     # ax6.plot(state_history['t'], state_history['powerConsumption6'])
#     # ax6.set_xlabel('time[sec]')
#     # ax6.set_ylabel('Power Consumption [W]')
#     # ax6.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
#     # ax6.grid()

#     plt.tight_layout()
#     plt.grid()
#     pass

## Plot the angular rates of the spacecraft ## 
fig = plt.figure()
for filename in os.listdir(path):
    state_history   = pd.read_csv( path + '/' + filename )
    ax1 = fig.add_subplot(2,2,1)
    ax2 = fig.add_subplot(2,2,2)
    ax3 = fig.add_subplot(2,2,3)
    ax4 = fig.add_subplot(2,2,4)

    #Plot angular velocity with respect to time. 
    ax1.plot(state_history['t'] / 60.0, state_history['w1'])
    # ax1.set_xlabel('time[sec]')
    ax1.set_ylabel('V_x[deg/sec]')
    ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax1.grid()

    ax2.plot(state_history['t'] / 60.0, state_history['w2'])
    # ax2.set_xlabel('time[sec]')
    ax2.set_ylabel('V_y [deg/sec]')
    ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax2.grid()

    ax3.plot(state_history['t'] / 60.0, state_history['w3'])
    ax3.set_xlabel('time[min]')
    ax3.set_ylabel('V_z [deg/sec]')
    ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax3.grid()

    ax4.plot(state_history['t'] / 60.0, state_history['slewRate'])
    ax4.set_xlabel('time[min]')
    ax4.set_ylabel('Slew Rate [deg/sec]')
    ax4.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
    ax4.grid()

    plt.tight_layout()
    plt.grid()
    pass 
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/results/angularRatesSpacecraft.svg')

# print "Figures generated successfully!"
# print ""
# # plt.plot() 

print "Figures generated successfully!"
print ""

# Stop timer
end_time = time.time( )

print ""
print "------------------------------------------------------------------"
print "                         Exited successfully!                     "
print "------------------------------------------------------------------"
print ""