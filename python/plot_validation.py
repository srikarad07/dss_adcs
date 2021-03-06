'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

## Validation of the single simulator case ##

# Set up modules and packages.
# Plotting
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

# Numerical
import numpy as np
import pandas as pd

# System
import sys
import time

# Set the style for the plots
# plt.style.use('seaborn-whitegrid')

print ""
print "------------------------------------------------------------------"
print "                             dss_adcs                             "
# print "                              0.1.0                               "
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
path            =  "/home/superman/Desktop/state_history_validation.csv"

state_history   = pd.read_csv( path )
 
print "Input data files successfully read!"

print "Figures being generated ..."

# Generate figure with 2D views.
fig = plt.figure( figsize=(5,6))
ax1 = fig.add_subplot(4, 1, 1)
ax2 = fig.add_subplot(4, 1, 2)
ax3 = fig.add_subplot(4, 1, 3)
ax4 = fig.add_subplot(4, 1, 4, frameon=True)

# Plot quaternion 1 with time.
ax1.plot(state_history['t']/60.0,state_history['q1'],color='k')
# ax1.scatter(0.0,0.0,s=100,marker='o',color='b')
# ax1.set_xlabel('time [min]')
ax1.set_title('q_1')
ax1.set_xlim(0, 15)
ax1.set_ylim(-0.5,0.5)
ax1.set_xticks([0, 5, 10, 15])
ax1.set_yticks([-0.5, 0, 0.5])
ax1.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax1.grid()

# Plot quaternion 1 with time.
ax2.plot(state_history['t']/60.0,state_history['q2'],color='k')
# ax1.scatter(0.0,0.0,s=100,marker='o',color='b')
# ax2.set_xlabel('time [min]')
ax2.set_title('q_2')
ax2.set_xlim(0, 15)
ax2.set_ylim(-0.5,0.5)
ax2.set_xticks([0, 5, 10, 15])
ax2.set_yticks([-0.5, 0, 0.5])
ax2.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax2.grid()

# Plot quaternion 1 with time.
ax3.plot(state_history['t']/60.0,state_history['q3'],color='k')
# ax3.scatter(0.0,0.0,s=100,marker='o',color='b')
# ax3.set_xlabel('time [min]')
ax3.set_title('q_3')
ax3.set_xlim(0, 15)
ax3.set_ylim(-1.0,1.0)
ax3.set_xticks([0, 5, 10, 15])
ax3.set_yticks([-1, 0, 1])
ax3.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax3.grid()

# Plot quaternion 1 with time.
ax4.plot(state_history['t']/60.0,state_history['q4'],color='k')
# ax4.scatter(0.0,0.0,s=100,marker='o',color='b')
ax4.set_xlabel('time [min]')
ax4.set_title('q_4')
ax4.set_xlim(0, 15)
ax4.set_ylim(0.4,1.5)
ax4.set_xticks([0, 5, 10, 15])
ax4.set_yticks([0.5, 1, 1.5])
ax4.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax4.grid()

plt.tight_layout()
fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/validation/quaternions_validation.eps')
#plt.close()
# plt.grid()

#Generate figure with 2D plots. 
fig = plt.figure( figsize=(5,6) )
ax1 = fig.add_subplot(3,1,1)
ax2 = fig.add_subplot(3,1,2)
ax3 = fig.add_subplot(3,1,3)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t']/60.0, state_history['controlTorque1'], color='k')
# ax1.set_xlabel('time[min]')
ax1.set_title('Control_X [N/m]')
ax1.set_xlim(0, 15)
ax1.set_xticks([0, 5, 10, 15])
ax1.set_ylim(-0.5,0.5)
ax1.set_yticks([-0.5, 0, 0.5])
ax1.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t']/60.0, state_history['controlTorque2'], color='k')
# ax2.set_xlabel('time[min]')
ax2.set_title('Control_Y [N/m]')
ax2.set_xlim(0, 15)
ax2.set_xticks([0, 5, 10, 15])
ax2.set_ylim(-0.5,0.5)
ax2.set_yticks([-0.5, 0, 0.5])
ax2.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t']/60.0, state_history['controlTorque3'], color='k')
ax3.set_xlabel('time[min]')
ax3.set_title('Control_Z [N/m]')
ax3.set_xlim(0, 15)
ax3.set_xticks([0, 5, 10, 15])
ax3.set_ylim(-0.5,0.5)
ax3.set_yticks([-0.5, 0, 0.5])
ax3.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax3.grid()

plt.tight_layout()

fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/validation/controlTorque_validation.eps')
# plt.close()

# Generate figure with 2D plots. 
fig = plt.figure( figsize=(5,6) )
ax1 = fig.add_subplot(4,1,1)
ax2 = fig.add_subplot(4,1,2)
ax3 = fig.add_subplot(4,1,3)
ax4 = fig.add_subplot(4,1,4)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t']/60.0, state_history['motorTorque4'], color='k')
# ax1.set_xlabel('time[min]')
ax1.set_title('Motor torque_1 [N/m]')
ax1.set_xlim(0, 15)
ax1.set_xticks([0, 5, 10, 15])
ax1.set_ylim(-0.5,0.5)
ax1.set_yticks([-0.5, 0, 0.5])
ax1.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t']/60.0, state_history['motorTorque3'], color='k')
# ax2.set_xlabel('time[min]')
ax2.set_title('Motor torque_2 [N/m]')
ax2.set_xlim(0, 15)
ax2.set_xticks([0, 5, 10, 15])
ax2.set_ylim(-0.4,0.4)
ax2.set_yticks([-0.4, 0, 0.4])
ax2.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t']/60.0, state_history['motorTorque2'], color='k')
# ax3.set_xlabel('time[min]')
ax3.set_title('Motor torque_3 [N/m]')
ax3.set_xlim(0, 15)
ax3.set_xticks([0, 5, 10, 15])
ax3.set_ylim(-0.4,0.4)
ax3.set_yticks([-0.4, 0, 0.4])
ax3.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax3.grid()

ax4.plot(state_history['t']/60.0, state_history['motorTorque1'], color='k')
ax4.set_xlabel('time[min]')
ax4.set_title('Motor torque_4 [N/m]')
ax4.set_xlim(0, 15)
ax4.set_xticks([0, 5, 10, 15])
ax4.set_ylim(-0.5,0.5)
ax4.set_yticks([-0.5, 0, 0.5])
ax4.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
ax4.grid()

plt.tight_layout()

fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/validation/motorTorque_validation.eps')
# plt.close()

#Generate control torque plots.
#Generate figure with 2D plots. 
fig = plt.figure( figsize=(5,6) )
ax1 = fig.add_subplot(4,1,1)
ax2 = fig.add_subplot(4,1,2)
ax3 = fig.add_subplot(4,1,3)
ax4 = fig.add_subplot(4,1,4, frameon=True)

#Plot angular velocity with respect to time. 
ax1.plot(state_history['t']/60.0, state_history['w1'], color='k')
# ax1.set_xlabel('time[min]')
ax1.set_title('w1[rad/sec]')
ax1.set_xlim(0, 15)
ax1.set_ylim(-0.002,0.002)
ax1.set_xticks([0, 10, 5, 15])
ax1.set_yticks([0, 0.002, -0.002])
ax1.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
ax1.grid()

ax2.plot(state_history['t']/60.0, state_history['w2'], color='k')
# ax2.set_xlabel('time[min]')
ax2.set_title('w2[rad/sec]')
ax2.set_xlim(0, 15)
ax2.set_ylim(-0.002,0.002)
ax2.set_xticks([0, 10, 5, 15])
ax2.set_yticks([0, 0.002, -0.002])
ax2.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
ax2.grid()

ax3.plot(state_history['t']/60.0, state_history['w3'], color='k')
# ax3.set_xlabel('time[min]')
ax3.set_title('w3 [rad/sec]')
ax3.set_xlim(0, 15)
ax3.set_ylim(-0.005,0.005)
ax3.set_xticks([0, 10, 5, 15])
ax3.set_yticks([0, 0.005, -0.005])
ax3.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
ax3.grid()

ax4.plot(state_history['t']/60.0, state_history['slewRate'], color='k')
ax4.set_xlabel('time[min]')
ax4.set_title('Slew Rate [deg/sec]')
ax4.set_xlim(0, 15)
ax4.set_ylim(0,0.4)
ax4.set_xticks([0, 10, 5, 15])
ax4.set_yticks([0, 0.2, 0.4])
ax4.ticklabel_format(style='plain', axis='y', scilimits=(0,0))
ax4.grid()

plt.tight_layout()
# plt.grid()

fig.savefig('/home/superman/Delft/Thesis/thesis-report/Images/validation/angularVelocity_validation.eps')
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
