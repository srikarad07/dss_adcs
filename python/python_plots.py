'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

# Set up modules and packages.
# Plotting
import matplotlib
# matplotlib.use('seaborn-whitegrid')
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

## Plotting function with system requirements in the plot. 
def plotWithSystemRequirements( xAxisParameterToPlot, yAxisParameterToPlot, system_requirements, ax ): 
    # sc =  ax.scatter( xAxisParameterToPlot, yAxisParameterToPlot, marker='.', linewidth=10 )
    
    ## Define the range over which the area will be shaded.
    xAxisParameterRange  = np.arange( 0.0, np.max(xAxisParameterToPlot), 1.0 ) 
    yAxisParameterRange  = np.arange( 0.0, np.max(yAxisParameterToPlot), 1.0 )
    
    ## Get the requirements of the system.
    if xAxisParameterToPlot.columns.values[0] in system_requirements.keys():
        xAxisParameteRequirement    = system_requirements[ xAxisParameterToPlot.columns.values[0] ]
    else: 
        xAxisParameteRequirement    = 0
        pass 

    if yAxisParameterToPlot.columns.values[0] in system_requirements.keys():    
        yAxisParameterRequirement   = system_requirements[ yAxisParameterToPlot.columns.values[0] ]
    else: 
        yAxisParameterRequirement   = 0
        pass 
    ## Check if the requirement if defined for the parameters that are being plotted. 
    if xAxisParameteRequirement != 0 and yAxisParameterRequirement != 0 :   
        ax.fill_between( xAxisParameterRange, yAxisParameterRequirement, 
                                     1.5*yAxisParameterRequirement, facecolor='yellow', alpha=0.4)
        ax.fill_betweenx( yAxisParameterRange, xAxisParameteRequirement, 
                                     1.5*xAxisParameteRequirement, facecolor='red', alpha=0.4)
    elif xAxisParameteRequirement != 0 and yAxisParameterRequirement == 0:
         ax.fill_betweenx( yAxisParameterRange, xAxisParameteRequirement, 
                                     1.5*xAxisParameteRequirement, facecolor='red', alpha=0.4)
    elif yAxisParameterRequirement != 0 and xAxisParameteRequirement == 0:
        ax.fill_between( xAxisParameterRange, yAxisParameterRequirement, 
                                     1.5*yAxisParameterRequirement, facecolor='yellow', alpha=0.4)
        pass 
    pass  

## Plotting function with system constraints in the plot. 
def plotWithSystemLevelConstraints(xParameterToPlot, yParameterToPlot, state_history, systemConstraints, ax): 
    
    # Get strings of the system constraint and the value defined by the user.
    constrainParameterString    = systemConstraints.keys()
    
    # Shorten the dataframe to only the value that are needed. 
    requiredStateHistory  = state_history[constrainParameterString] 
    
    # Check if all rows are non-zero. Is used later to assess the final 
    # criteria for assessment of system constraints. 
    finalCriteria     = requiredStateHistory.all(axis=1)

    for ii in range(len(constrainParameterString)):
        
        ## Get the constrina values and testing criteria for the system constraint.
        intermediateConstraintString    =  constrainParameterString[ii]
        intermediateConstraintValue     =  systemConstraints[constrainParameterString[ii]]
        intermediateCriteria            = requiredStateHistory[intermediateConstraintString] < intermediateConstraintValue
        
        # Update the criteria with the criteria obtained for non zero with the system 
        # constraint criteria above.
        finalCriteria = finalCriteria & intermediateCriteria
        pass 

    print "The final concept values: ", requiredStateHistory[finalCriteria]
    
    ## Obtain the values that satisfy the constraints. 
    constrainedValues           = requiredStateHistory[finalCriteria] 

    # Get the index of values below constraints
    indexOfBelowConstraints             = constrainedValues.index.values
    
    ## Get the x axis and y axis parameters for the values below the the constraints. 
    constrainedXAxisParameterToPlot     = xParameterToPlot.loc[indexOfBelowConstraints]
    constrainedYAxisParameterToPlot     = yParameterToPlot.loc[indexOfBelowConstraints]

    return constrainedXAxisParameterToPlot, constrainedYAxisParameterToPlot

def dss_adcs_plotter( path, stringToBeLocated, plotWithString, pltTitles, axisTitles, systemRequirements, 
                      systemConstraints, showFigureBool, saveFigureBool, pathToSaveFigure, subplots, 
                      figureSize, hoverFlag, font ): 
    
    # Font for matplotlib 
    matplotlib.rc('font', **font) 

    # String that will be used to obtain the state history. 
    stateHistoryString          = 'state_history'

    # String that will be used to obtain the metadata. 
    metadataString              = 'metadata'

    ## For hover options. 
    norm    = plt.Normalize(1,4)
    cmap    = plt.cm.RdYlGn
    
    ## System requirements. 
    system_requirements  = np.array(systemRequirements)

    ## System constraints. 
    system_constraints   = np.array(systemConstraints)

    ## Search for the files with the string as given in the path. 
    if plotWithString == 'none': 
        filesForTheplots    = requiredFiles( path, stateHistoryString )
        filesForThePlots2   = requiredFiles( path, metadataString )
    else:
        filesForTheplots    = requiredFiles( path, stateHistoryString, plotWithString )
        filesForThePlots2   = requiredFiles( path, metadataString, plotWithString )        
        pass
    
    ## Loop over the files to plot parameters from all the files. 
    fig = plt.figure( figsize=figureSize )
    for filename in range(len(filesForTheplots)):

        state_history   = pd.read_csv( filesForTheplots[filename] )
        metadata        = pd.read_csv( filesForThePlots2[filename] )

        stateHistoryPath        = filesForTheplots[filename]
        metadataPath            = filesForThePlots2[filename]

        if subplots == True: 
            
            yAxisParameter  = stringLocator( stateHistoryPath, stringToBeLocated[0] )
            xAxisParameter  = stringLocator( stateHistoryPath, stringToBeLocated[1] )
            jj = np.ceil( len(yAxisParameter)/ 2.0 )

            for i in range(len(yAxisParameter)):
                ax = fig.add_subplot(2,jj,i+1)
                ax.plot( state_history[xAxisParameter]/60.0, state_history[yAxisParameter[i]] )
                # ax.set_title( 'Power' + str(i+1) + ' [W]' )
                ax.set_xlabel('time[min]')
                handles, labels = ax.get_legend_handles_labels()
                # ax.legend(handles, labels)
                ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
                pass 
            plt.suptitle( pltTitles, fontsize=16 )
            plt.tight_layout()
            plt.grid()
            pass 
        
        elif subplots == False:            
            # Update the index of dataframe to be the concept identifier
            state_history.set_index('ConceptIdentifier', inplace=True)
            metadata.set_index('ConceptIdentifier', inplace=True)

            ## Check if the indeces of the state history and metadata are same!
            if False == np.array_equal( state_history.index.values, metadata.index.values): 
                print "ERROR!! Metadata and state history indexes do not match!!" 
                exit()

            yAxisParameterString  = stringLocator( stateHistoryPath, stringToBeLocated[0] )
            xAxisParameterString  = stringLocator( metadataPath, stringToBeLocated[1] )
            if xAxisParameterString.size == 0:
                xAxisParameterString2  = stringLocator( stateHistoryPath, stringToBeLocated[1] )
                pass 

            # To be displayed over the scatter points when hovered upon.
            names                   = state_history.index.values
            
            ax = fig.add_subplot(111)
            for ii in range(len(yAxisParameterString)):
                ## Add the extra [] to get the result as a dataframe; it is used 
                ## later for a function! 
                yAxisParameterToPlot  = state_history[[yAxisParameterString[ii]]] 
                if xAxisParameterString.size == 0:
                   xAxisParameterToPlot  = state_history[xAxisParameterString2] 
                else:    
                   xAxisParameterToPlot  = metadata[xAxisParameterString] 
                   pass 
                ax.set_xlabel(axisTitles[1])
                ax.set_ylabel(axisTitles[0])

                ## Check if the system requirements are defined by the user. 
                if  bool(systemRequirements) == 1 and bool(systemConstraints) != 1:
                    sc =  ax.scatter( xAxisParameterToPlot, yAxisParameterToPlot, marker='.', linewidth=10 )

                    plotWithSystemRequirements( xAxisParameterToPlot, yAxisParameterToPlot, systemRequirements, ax )

                ## Check if the system constraints are defined by the user. 
                elif bool(systemConstraints) == 1 and bool(systemRequirements) != 1: 
                    
                    ## Check for system constraints
                    constrainedXAxisParameterToPlot, constrainedYAxisParameterToPlot = plotWithSystemLevelConstraints( xAxisParameterToPlot, 
                                                    yAxisParameterToPlot, 
                                                    state_history,
                                                    systemConstraints, ax )
                    sc =  ax.scatter( constrainedXAxisParameterToPlot,
                                      constrainedYAxisParameterToPlot, 
                                      marker='.', linewidth=10 ) 
                    
                elif bool(systemConstraints) == 1 and bool(systemRequirements) == 1: 
                    
                    ## Check for system constraints
                    constrainedXAxisParameterToPlot, constrainedYAxisParameterToPlot = plotWithSystemLevelConstraints( xAxisParameterToPlot, 
                                                    yAxisParameterToPlot, 
                                                    state_history,
                                                    systemConstraints, ax )
                    sc =  ax.scatter( constrainedXAxisParameterToPlot,
                                      constrainedYAxisParameterToPlot, 
                                      marker='.', linewidth=10 ) 
                    plotWithSystemRequirements( constrainedXAxisParameterToPlot, 
                                                constrainedYAxisParameterToPlot, 
                                                systemRequirements, ax )
                    
                else: 
                    sc =  ax.scatter( xAxisParameterToPlot, 
                                      yAxisParameterToPlot, 
                                      marker='.', linewidth=10 )
                    pass 
                ax.autoscale()
                ax.grid(linestyle='--', linewidth=0.25, color='black')
                ax.ticklabel_format(style='plain', axis='both')
                c       = np.random.randint(1,5,size=len(xAxisParameterToPlot))
                plt.tight_layout()
                # plt.draw( )
                
                ## Display concept identifier upone hovering over the plot.
                if hoverFlag:
                    annot   = ax.annotate("", xy=(0,0), xytext=(20,20),textcoords="offset points",
                    bbox=dict(boxstyle="round", fc="w"),
                    arrowprops=dict(arrowstyle="->"))
                    annot.set_visible(False)
                    ## <<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>> ## 
                    ## Figure out a way to move it to a separate function ## 
                    # Hover over the point to get coordinates. 
                    def update_annot(ind, sc, annot):
                        pos = sc.get_offsets()[ind["ind"][0]]
                        annot.xy = pos
                        text = "{}".format( " ".join([names[n] for n in ind["ind"]]))
                        annot.set_text(text)
                        annot.get_bbox_patch().set_facecolor(cmap(norm(c[ind["ind"][0]])))
                        annot.get_bbox_patch().set_alpha(0.4)
                        pass 
                    def hover(event):
                        vis = annot.get_visible()
                        if event.inaxes == ax:
                            cont, ind = sc.contains(event)
                            if cont:
                                update_annot(ind, sc, annot)
                                annot.set_visible(True)
                                fig.canvas.draw_idle()
                            else:
                                if vis:
                                    annot.set_visible(False)
                                    fig.canvas.draw_idle()
                        pass 
                    norm    = plt.Normalize(1,4)
                    cmap    = plt.cm.RdYlGn
                    ## <<<<<<<<<<<<<<<<<<<<, TO DO >>>>>>>>>>>>>>>>> ## 
                    fig.canvas.mpl_connect("motion_notify_event", hover)
                    pass 
                pass 
            pass 
        pass
    
    if saveFigureBool: 
        fig.savefig( pathToSaveFigure + stringToBeLocated[0] + "VS" + stringToBeLocated[1] + '.eps'  )
        pass 
    # plt.show()
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
stringToPlot                = np.array( inputPythonPlotData["parameters_to_plot"] )

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

# Figure Size
figureSize                  = inputPythonPlotData["figSize"]

# Hover Flag                
hoverFlag                   = inputPythonPlotData["hover_flag"] 

# Font type and size: 
font                        = inputPythonPlotData["font"]

# Requirements of the system: 
system_requirements         = inputPythonPlotData["systemRequirements"]

# Constraints on the system: 
system_constraints          = inputPythonPlotData["systemConstraints"]

for i in range( len(stringToPlot) ): 

    dss_adcs_plotter( stateHistoryFilePath, stringToPlot[i], plotWithString, plotTitles[i], axesTitles[i], system_requirements, system_constraints, showFigure, savefig, saveFigurePath, subplots, figureSize, hoverFlag, font )
    
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
