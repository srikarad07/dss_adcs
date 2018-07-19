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
from matplotlib import rcParams
from matplotlib import cm
from matplotlib.font_manager import FontProperties
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d import axes3d
import matplotlib.animation as animation

# I/O
# import commentjson
import json
from pprint import pprint
import sqlite3

# Numerical
import math
import numpy as np
import pandas as pd

# System
import sys
import time

print ""
print "------------------------------------------------------------------"
print "                             dss_adcs                             "
# print "                              0.1.0                               "
print "      Copyright (c) 2018, S.D. Cherukuri (srikaradd007@gmail.com)  "
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
path            =  "/home/superman/Desktop/state_history.csv"
# path            =  "/home/superman/Desktop/state_history_concept_1_4.csv"
state_history   = pd.read_csv( path )
 
print "Input data files successfully read!"

print "Figures being generated ..."

# Generate figure with 2D views.
fig = plt.figure()
ax1 = fig.add_subplot(4, 1, 1)
ax2 = fig.add_subplot(4, 1, 2)
ax3 = fig.add_subplot(4, 1, 3)
ax4 = fig.add_subplot(4, 1, 4, frameon=False)

# Plot quaternion 1 with time.
ax1.plot(state_history['t'],state_history['q1'],color='k')
# ax1.scatter(0.0,0.0,s=100,marker='o',color='b')
ax1.set_xlabel('time [sec]')
ax1.set_ylabel('q_1')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

# Plot quaternion 1 with time.
ax2.plot(state_history['t'],state_history['q2'],color='k')
# ax1.scatter(0.0,0.0,s=100,marker='o',color='b')
ax2.set_xlabel('time [sec]')
ax2.set_ylabel('q_2')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

# Plot quaternion 1 with time.
ax3.plot(state_history['t'],state_history['q3'],color='k')
# ax3.scatter(0.0,0.0,s=100,marker='o',color='b')
ax3.set_xlabel('time [sec]')
ax3.set_ylabel('q_3')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

# Plot quaternion 1 with time.
ax4.plot(state_history['t'],state_history['q4'],color='k')
# ax4.scatter(0.0,0.0,s=100,marker='o',color='b')
ax4.set_xlabel('time [sec]')
ax4.set_ylabel('q_4')
ax4.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax4.grid()

plt.tight_layout()
plt.grid()

# #Generate figure with 2D plots. 
# fig = plt.figure()
# ax1 = fig.add_subplot(3,1,1)
# ax2 = fig.add_subplot(3,1,2)
# ax3 = fig.add_subplot(3,1,3)

# #Plot angular velocity with respect to time. 
# ax1.plot(state_history['t'], state_history['theta1'], color='k')
# ax1.set_xlabel('time[sec]')
# ax1.set_ylabel('theta_1 [deg]')
# ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
# ax1.grid()

# ax2.plot(state_history['t'], state_history['theta2'], color='k')
# ax2.set_xlabel('time[sec]')
# ax2.set_ylabel('theta_2 [deg]')
# ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
# ax2.grid()

# ax3.plot(state_history['t'], state_history['theta3'], color='k')
# ax3.set_xlabel('time[sec]')
# ax3.set_ylabel('theta_3 [deg]')
# # ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
# ax3.grid()

# plt.tight_layout()
# plt.grid()

#Generate figure with 2D plots. 
fig = plt.figure()
ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t'], state_history['controlTorque1'], color='k')
ax1.set_xlabel('time[sec]')
ax1.set_ylabel('Control_X [N/m]')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t'], state_history['controlTorque2'], color='k')
ax2.set_xlabel('time[sec]')
ax2.set_ylabel('Control_Y [N/m]')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t'], state_history['controlTorque3'], color='k')
ax3.set_xlabel('time[sec]')
ax3.set_ylabel('Control_Z [N/m]')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

plt.tight_layout()
plt.grid()

# Generate figure with 2D plots. 
fig = plt.figure()
ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t'], state_history['motorTorque1'], color='k')
ax1.set_xlabel('time[sec]')
ax1.set_ylabel('Motor torque_X [N/m]')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t'], state_history['motorTorque2'], color='k')
ax2.set_xlabel('time[sec]')
ax2.set_ylabel('Motor torque_X [N/m]')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t'], state_history['motorTorque3'], color='k')
ax3.set_xlabel('time[sec]')
ax3.set_ylabel('Motor torque_X [N/m]')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

plt.tight_layout()
plt.grid()

# Generate figure with 2D plots. 
fig = plt.figure()
ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t'], state_history['angularMomentum1'], color='k')
ax1.set_xlabel('time[sec]')
ax1.set_ylabel('Angular momentum [kgm2/sec]')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t'], state_history['angularMomentum2'], color='k')
ax2.set_xlabel('time[sec]')
ax2.set_ylabel('Angular momentum [kgm2/sec]')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t'], state_history['angularMomentum3'], color='k')
ax3.set_xlabel('time[sec]')
ax3.set_ylabel('Angular momentum [kgm2/sec]')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

plt.tight_layout()
plt.grid()

# Generate figure with 2D plots. 
fig = plt.figure()
ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t'], state_history['reactionWheelAngularVelocity1'], color='k')
ax1.set_xlabel('time[sec]')
ax1.set_ylabel('Angular Velocity [rad/sec]')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t'], state_history['reactionWheelAngularVelocity2'], color='k')
ax2.set_xlabel('time[sec]')
ax2.set_ylabel('Angular Velocity [rad/sec]')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t'], state_history['reactionWheelAngularVelocity3'], color='k')
ax3.set_xlabel('time[sec]')
ax3.set_ylabel('Angular Velocity [rad/sec]')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

plt.tight_layout()
plt.grid()

# Generate figure with 2D plots. 
fig = plt.figure()
ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t'], state_history['powerConsumption1'], color='k')
ax1.set_xlabel('time[sec]')
ax1.set_ylabel('Power Consumption [W]')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t'], state_history['powerConsumption2'], color='k')
ax2.set_xlabel('time[sec]')
ax2.set_ylabel('Power Consumption [W]')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t'], state_history['powerConsumption3'], color='k')
ax3.set_xlabel('time[sec]')
ax3.set_ylabel('Power Consumption [W]')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

plt.tight_layout()
plt.grid()

#Generate control torque plots.
#Generate figure with 2D plots. 
fig = plt.figure()
ax1 = fig.add_subplot(4,1,1)
ax2 = fig.add_subplot(4,1,2)
ax3 = fig.add_subplot(4,1,3)
ax4 = fig.add_subplot(4,1,4)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t'], state_history['w1'], color='k')
ax1.set_xlabel('time[sec]')
ax1.set_ylabel('V_x[deg/sec]')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t'], state_history['w2'], color='k')
ax2.set_xlabel('time[sec]')
ax2.set_ylabel('V_y [deg/sec]')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t'], state_history['w3'], color='k')
ax3.set_xlabel('time[sec]')
ax3.set_ylabel('V_z [deg/sec]')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

ax4.plot(state_history['t'], state_history['slewRate'], color='k')
ax4.set_xlabel('time[sec]')
ax4.set_ylabel('Slew Rate [deg/sec]')
ax4.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax4.grid()

plt.tight_layout()
plt.grid()

#Generate disturbance torque plots.
#Generate figure with 2D plots. 
fig = plt.figure()
ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t'], state_history['disturbanceTorque1'], color='k')
ax1.set_xlabel('time[sec]')
ax1.set_ylabel('T_dx')
ax1.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t'], state_history['disturbanceTorque2'], color='k')
ax2.set_xlabel('time[sec]')
ax2.set_ylabel('T_Dy')
ax2.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t'], state_history['disturbanceTorque3'], color='k')
ax3.set_xlabel('time[sec]')
ax3.set_ylabel('T_Dz')
ax3.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
ax3.grid()

plt.tight_layout()
plt.grid()

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