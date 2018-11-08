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

def dss_adcs_plotter( path, stringToBeLocated, plotWithString, pltTitles, axisTitles, showFigureBool, saveFigureBool, pathToSaveFigure, subplots, figureSize, hoverFlag, font ): 
    
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
            if False == np.array_equal( state_history.index.values, metadata.index.values): 
                print("ERROR!! Metadata and state history indexes do not match!!")
                exit()

            yAxisParameterString  = stringLocator( stateHistoryPath, stringToBeLocated[0] )
            xAxisParameterString  = stringLocator( metadataPath, stringToBeLocated[1] )
            # print("String searched with: ", stringToBeLocated[0])
            # print("Strings for the plot: ", yAxisParameterString, xAxisParameterString )

            # To be displayed over the scatter points when hovered upon.
            names                   = state_history.index.values
            
            ax = fig.add_subplot(111)
            for ii in range(len(yAxisParameterString)):
                yAxisParameterToPlot  = state_history[yAxisParameterString[ii]] 
                xAxisParameterToPlot  = metadata[xAxisParameterString] 
                sc =  ax.scatter( xAxisParameterToPlot, yAxisParameterToPlot, marker='.', linewidth=10 )
                ax.set_xlabel(axisTitles[1])
                ax.set_ylabel(axisTitles[0])
                ax.grid(linestyle='--', linewidth=0.25, color='black')
                ax.ticklabel_format(style='plain', axis='both', scilimits=(0,0))
                c       = np.random.randint(1,5,size=len(xAxisParameterToPlot))
                plt.tight_layout()
                plt.draw( )
                
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
        fig.savefig( pathToSaveFigure + stringToBeLocated )
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

for i in range( len(stringToPlot) ): 

    dss_adcs_plotter( stateHistoryFilePath, stringToPlot[i], plotWithString, plotTitles[i], axesTitles[i], showFigure, savefig, saveFigurePath, subplots, figureSize, hoverFlag, font )
    
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

