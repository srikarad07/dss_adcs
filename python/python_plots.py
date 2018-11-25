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

# Import the plotter functions 
from plotter_functions import plotWithSystemRequirements
from plotter_functions import plotWithSystemLevelConstraints
from plotter_functions import hoverFunctionality
from plotter_functions import plotWithSystemConstraintsAndRequirements
from plotter_functions import singleSimulationPlots
from plotter_functions import bulkSimulationPlots

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

def dss_adcs_plotter( path, stringToBeLocated, mode, typeOfPlots, plotWithString, pltTitles, axisTitles, 
systemRequirements, systemConstraints, showFigureBool, saveFigureBool, pathToSaveFigure, subplotFlag, 
                      figureProperties, hoverFlag, font, plotProperties ): 
    
    # Number of simulations.
    # <<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>> ##
    # This should be defined by the user in the json input file   
    numberOfSimulations     = 100
    # <<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>> ##

    # plot_properties =  {
    #     "marker":".", 
    #     "linewidth":10.0, 
    #     "c":"cyan",
    #     "alpha":0.5 
    # "font" : {
    #     "family" : "monospace",
    #     "weight" : "bold",
    #     "size"   :  15 
    #     },
    # }
    # "fig_properties"  : {
    # "figsize" :  [ 10.0, 
    #             8.0
    #         ],
    # "tight_layout" : true
    # }, 

    plt.style.use('dark_background')
    # Font for matplotlib 
    matplotlib.rc('font', **font) 

    # String that will be used to obtain the state history. 
    stateHistoryString          = 'state_history'

    # String that will be used to obtain the metadata. 
    metadataString              = 'metadata'

    ## For hover options. 
    norm    = plt.Normalize(1,4)
    cmap    = plt.cm.RdYlGn

    ## Search for the files with the string as given in the path. 
    if plotWithString == 'none': 
        filesForTheplots    = requiredFiles( path, stateHistoryString )
        filesForThePlots2   = requiredFiles( path, metadataString )
    else:
        filesForTheplots    = requiredFiles( path, stateHistoryString, plotWithString )
        filesForThePlots2   = requiredFiles( path, metadataString, plotWithString )        
        pass
  
    fig = plt.figure( **figureProperties )
    ax  = fig.add_subplot(111)
    ax.autoscale()
    ax.grid(linestyle='--', linewidth=0.25, color='black')
    
    ## <<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>> ## 
    # Switching off the scientific tick format is not working! Need to be fixed 
    ax.tick_params(axis='both', which='major')
    plt.ticklabel_format(axis='both', style='sci', scilimits=None)    
    ## <<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>> ## 
    
    ax.set_xlabel(axisTitles[1])
    ax.set_ylabel(axisTitles[0])

    ## Loop over the files to plot parameters from all the files. 
    for filename in range(len(filesForTheplots)):

        state_history           = pd.read_csv( filesForTheplots[filename] )
        metadata                = pd.read_csv( filesForThePlots2[filename] )

        stateHistoryPath        = filesForTheplots[filename]
        metadataPath            = filesForThePlots2[filename]
            
        # Update the index of dataframe to be the concept identifier
        state_history.set_index('ConceptIdentifier', inplace=True)
        metadata.set_index('ConceptIdentifier', inplace=True)

        ## Check if the indexes of the state history and metadata are same!
        if False == np.array_equal( state_history.index.values, metadata.index.values): 
            print "ERROR!! Metadata and state history indexes do not match!!" 
            exit()
        
        ## Get the xAxis and yAxis parameter strings
        yAxisParameterString  = stringLocator( stateHistoryPath, stringToBeLocated[0] )
        xAxisParameterString  = stringLocator( metadataPath, stringToBeLocated[1] )    
        if xAxisParameterString.size == 0:
            xAxisParameterString2  = stringLocator( stateHistoryPath, stringToBeLocated[1] )
            pass 

        # # To be displayed over the scatter points when hovered upon.
        # names                   = state_history.index.values

        if mode == "single_simulation": 
            
            yAxisParameterString  = stringLocator( stateHistoryPath, stringToBeLocated[0] )
            xAxisParameterString  = stringLocator( stateHistoryPath, stringToBeLocated[1] )
            # jj = np.ceil( len(yAxisParameter)/ 2.0 )

            ax = singleSimulationPlots(subplotFlag, yAxisParameterString, xAxisParameterString, fig, state_history  )
            plt.suptitle( pltTitles, fontsize=16 )
            plt.tight_layout()
            plt.grid()
            pass 
        
        elif mode == "bulk_simulation":            
                                
            bulkSimulationPlots( yAxisParameterString, xAxisParameterString, xAxisParameterString2, state_history, metadata, ax, systemRequirements, systemConstraints, hoverFlag, fig, typeOfPlots, numberOfSimulations, plotProperties )

            pass 

        pass
    if saveFigureBool: 
        fig.savefig( pathToSaveFigure + stringToBeLocated[0] + "VS" + stringToBeLocated[1] + '.eps'  )
        pass 
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
numberOfReactionWheels      = inputPythonPlotData["plot_with_string"]
if (numberOfReactionWheels != "none"):
    plotWithString              = numberOfReactionWheels 
else: 
    plotWithString              = "none"
    pass 

# String to be located.
stringToPlot                = np.array( inputPythonPlotData["parameters_to_plot"] )

# Simulation mode. 
mode                        = inputPythonPlotData["mode"]

# Type of plots. 
typeOfPlots                 = inputPythonPlotData["type_of_plots"]

# Titles of the figure.
plotTitles                  = inputPythonPlotData["pltTitles"]

# Axis titles. 
axesTitles                  = inputPythonPlotData["axisTitles"]

# Show figure bool True | False 
showFigure                  = inputPythonPlotData["showFigures"]

# Save figure bool True | False 
savefig                     = inputPythonPlotData["saveFigures"]

# Save figure path 
saveFigurePath              = inputPythonPlotData["saveFigurePath"]

# Subplots or same plot
subplots                    = False

# Figure Poperties
figureProperties            = inputPythonPlotData["fig_properties"]

# Hover Flag                
hoverFlag                   = inputPythonPlotData["hover_flag"] 

# Font type and size: 
font                        = inputPythonPlotData["font"]

# Properties of the plot: 
plotProperties              = inputPythonPlotData["plot_properties"]

# Requirements of the system: 
system_requirements         = inputPythonPlotData["systemRequirements"]

# Constraints on the system: 
system_constraints          = inputPythonPlotData["systemConstraints"]

for i in range( len(stringToPlot) ): 

    dss_adcs_plotter( stateHistoryFilePath, stringToPlot[i], mode, typeOfPlots, plotWithString, plotTitles[i], axesTitles[i], system_requirements, system_constraints, showFigure, savefig, saveFigurePath, subplots, figureProperties, hoverFlag, font, plotProperties )
    
    pass 
    
plt.show( showFigure )

print "Figures generated successfully!"
print ""

# Stop timer
end_time = time.time( )

print ""
print "------------------------------------------------------------------"
print "                         Exited successfully!                     "
print "------------------------------------------------------------------"
print ""

# ### 
#     "parameters_to_plot": [
#         [ "settlingTime", "principleInertia2" ], 
#         [ "systemPeakPower", "principleInertia2"],
#         [ "systemAvgPower", "principleInertia2"]  
#     ],
#     "axisTitles" : [ 
#         ["Maneuver time [min]", "MOIx [kg/m^2]" ], 
#         ["Peak power [W]", "MOIx [kg/m^2]"],
#         ["Average power [W]", "MOIx [kg/m^2]"]
#     ],

    # "parameters_to_plot": [
    #     [ "settlingTime", "SlewRate" ], 
    #     [ "systemPeakPower", "SlewRate"],
    #     [ "systemAvgPower", "SlewRate"]  
    # ],
    # "axisTitles" : [ 
    #     ["Maneuver time [min]", "Slew rate [deg/sec]" ], 
    #     ["Peak power [W]", "Slew rate [deg/sec]"],
    #     ["Average power [W]", "Slew rate [deg/sec]"]
    # ],
