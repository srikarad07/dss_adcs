'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

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
import json
from pprint import pprint
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

def dss_adcs_plotter( path, stringToBeLocated, plotWithString, pltTitles, showFigureBool, saveFigureBool, pathToSaveFigure, subplots ): 

    stringToSearchWith          = 'state_history'
    if plotWithString == 'none': 
        filesForTheplots    = requiredFiles( path, stringToSearchWith )
    else:
        filesForTheplots    = requiredFiles( path, stringToSearchWith, plotWithString )
        pass
    fig = plt.figure()
    
    for filename in range(len(filesForTheplots)):
        state_history   = pd.read_csv( filesForTheplots[filename] )
        testPath        = filesForTheplots[filename]
        results = stringLocator(testPath, stringToBeLocated )
        jj = np.ceil( len(results)/ 2.0 )

        if subplots == True: 
            for i in range(len(results)):
                ax = fig.add_subplot(2,jj,i+1)
                ax.plot( state_history['t']/60.0, state_history[results[i]] )
                # ax.set_title( 'Power' + str(i+1) + ' [W]' )
                ax.set_xlabel('time[min]')
                handles, labels = ax.get_legend_handles_labels()
                # ax.legend(handles, labels)
                ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
                pass 
            plt.suptitle( pltTitles, fontsize=16 )
            plt.tight_layout()
            plt.grid()
         
        elif subplots == False: 
            pass 
        pass
    
    if saveFigureBool: 
        fig.savefig( pathToSaveFigure + stringToBeLocated )
        pass 
    # plt.close( showFigureBool )
    plt.show( showFigureBool )
    pass 

## JSON file path. 
pythonInputsJsonPath    = "/home/superman/workspace/dss_adcs/python/plot_inputs.json"

## Read the json file. 
with open( pythonInputsJsonPath ) as data_file: 
    inputPythonPlotData     = json.load(data_file)
    pass 

# Path of the state history file. 
stateHistoryFilePath        = inputPythonPlotData["filePath"]

# Plot with searching for a specific string.
numberOfReactionWheels      = inputPythonPlotData["number_of_reactionWheels"]
if (numberOfReactionWheels != "none"):
    plotWithString              = numberOfReactionWheels + '.csv' 
else: 
    plotWithString              = "none"
    pass 

# String to be located.
stringToPlot                = inputPythonPlotData["parameters_to_plot"]

# Titles of the figure.
plotTitles                  = inputPythonPlotData["pltTitles"]

# Show figure bool True | False 
showFigure                  = inputPythonPlotData["showFigures"]

# Save figure bool True | False 
savefig                     = inputPythonPlotData["saveFigures"]

# Save figure path 
saveFigurePath              = inputPythonPlotData["saveFigurePath"]

# Subplots or same plot
subplots                    = True

for i in range( len(stringToPlot) ): 

    dss_adcs_plotter( stateHistoryFilePath, stringToPlot[i], plotWithString, plotTitles[i], showFigure, savefig, saveFigurePath, subplots )
    
    pass 

print "Figures generated successfully!"
print ""

# Stop timer
end_time = time.time( )

print ""
print "------------------------------------------------------------------"
print "                         Exited successfully!                     "
print "------------------------------------------------------------------"
print ""

