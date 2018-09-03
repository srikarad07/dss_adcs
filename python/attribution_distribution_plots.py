'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

# Set up modules and packages.
# Plotting
import matplotlib
import matplotlib.pyplot as plt

# Numerical
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

pathToFile      = "/home/superman/Desktop/productAttributesData/metadata.csv"
metadata        = pd.read_csv( pathToFile )

n_bins = 20

# Draw mass distribution plots. 
mass             = metadata['mass']
fig, axs         = plt.subplots( 1, 1, tight_layout=True )
n, bins, patches = axs.hist( mass, bins=n_bins)
axs.set_xlabel('Mass [kg]')
axs.set_ylabel('Count')
axs.set_title('Mass distribution reaction wheels')
# plt.grid()
# plt.box()
plt.close()

# Draw torque distribution plots. 
torque          = metadata['torque']
fig2, ax2       = plt.subplots(1, 1, tight_layout=True)
ax2.hist(torque, bins=n_bins)
ax2.set_xlabel('Torque [Nm]')
ax2.set_ylabel('Count')
plt.close()

# Draw momentum storage distribution plots. 
momentumStorage = metadata['momentumStorage']
fig3, ax3       = plt.subplots(1, 1, tight_layout=True)
ax3.hist(momentumStorage, bins=n_bins)
ax3.set_xlabel('Momentum storage [Nmsec]')
ax3.set_ylabel('Count')
plt.grid()
plt.close()

# Draw Volume distribution plot
volume          = metadata['volume']
fig4, ax4       = plt.subplots(1, 1, tight_layout=True)
ax4.hist(volume, bins=n_bins)
plt.grid()
plt.close()

# Draw moments of inertia distribution plot. 

# Log plot of Torque vs Momentum storage. 
fig5, ax5       = plt.subplots(1, 1, tight_layout=True ) 
ax5.set_xscale( "log" )
ax5.set_yscale( "log" )
ax5.scatter( momentumStorage, torque)
ax5.set_xlabel('Momentum Storage [Nms]')
ax5.set_ylabel('Torque [Nm]')
ax5.set_title('Momentum Storage vs Torque for reaction wheels')
# Customize the major grid
ax5.grid(which='major', linestyle='-', linewidth='0.5', color='red')
# Customize the minor grid
ax5.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
plt.close(False)

# Log plot of torque vs mass 
fig, ax     = plt.subplots(1, 1, tight_layout=True)
ax.set_xscale("log")
ax.set_yscale("log")
ax.scatter(torque, mass)
ax.set_xlabel('Torque [Nm]')
ax.set_ylabel('Mass [kg]')
ax.set_title('Torque vs Mass for reaction wheels')
# Customize the major grid
ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
# Customize the minor grid
ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
plt.close()

# Log plot of torque vs angular momentum/mass 
fig, ax     = plt.subplots(1, 1, tight_layout=True)
ax.set_xscale("log")
ax.set_yscale("log")
ax.scatter(momentumStorage/(mass), torque)
ax.set_xlabel('Momentum storage / mass Nms/kg')
ax.set_ylabel('Torque [Nm]')
ax.set_title('Momentum storage per unit mass vs Torque')
# Customize the major grid
ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
# Customize the minor grid
ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
plt.close(False)

# Log plot of momentum storage/mass vs angular momentum 
fig, ax     = plt.subplots(1, 1, tight_layout=True)
ax.set_xscale("log")
ax.set_yscale("log")
ax.scatter(momentumStorage, momentumStorage/(mass))
ax.set_xlabel('Momentum storage mass Nms')
ax.set_ylabel('Momentum storage / mass Nms/kg')
ax.set_title('Momentum storage per unit mass vs Momentum storage')
# Customize the major grid
ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
# Customize the minor grid
ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
plt.close(False)

# Show plots 
plt.show()