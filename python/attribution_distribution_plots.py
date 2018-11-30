'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

# Set up modules and packages.
# Plotting
import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns 
import matplotlib.patches as mpatches
import matplotlib.collections as mcollections

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

# Hover functionality 
from plotter_functions import hoverFunctionality

# plt.style.use('seaborn-white')

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

# Change the font size 
font = {'family' : 'monospace',
        'weight' : 'bold',
        'size'   :  15}
matplotlib.rc('font', **font)        
matplotlib.rc('xtick', labelsize=13)
matplotlib.rc('ytick', labelsize=13)

pathToFile      = "/home/superman/Desktop/productAttributesData/metadata.csv"
metadata        = pd.read_csv( pathToFile )

# saveFigPath     = "/home/superman/Delft/Thesis/IAC/iac_2018/Images/imd/"
saveFigPath       = "/home/superman/Delft/Thesis/thesis-report/Images/data/"

# numnber of bins for histogram
n_bins = 4

# Close plot 
closePlot       = False 

# Type of plot 
# typeOfPlot      = "scatter"
typeOfPlot      = "scatter"
# typeOfPlot      = "general"

## Update the index of the metadata file 
metadata.set_index('name', inplace=True)

peakPower       = metadata['maxPower']
mass            = metadata['mass']
momentum        = metadata['angularMomentum']
maxTorque       = metadata['maxTorque'] 
torque          = metadata['torque']
maxMomentum     = metadata['maxMomentumStorage']
volume          = metadata['volume']
names           = metadata.index.values

## Histograms: 
if typeOfPlot == "histogram": 
        
        fig, axs         = plt.subplots( 1, 1, tight_layout=True )
        n, bins, patches = axs.hist( mass, bins=n_bins)
        axs.set_xlabel('Mass [kg]')
        axs.set_ylabel('Count')
        plt.grid()
        # axs.set_title('Mass distribution reaction wheels')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'mass_distribution.eps')

        # Draw torque distribution plots. 
        torque          = metadata['torque']
        fig2, ax2       = plt.subplots(1, 1, tight_layout=True)
        ax2.hist(torque.dropna(), bins=n_bins)
        ax2.set_xlabel('Torque [Nm]')
        ax2.set_ylabel('Count')
        plt.grid()
        plt.close(closePlot)
        fig2.savefig(saveFigPath + 'torque_distribution.eps')

        # Draw momentum storage distribution plots. 
        momentumStorage = metadata['angularMomentum']
        fig3, ax3       = plt.subplots(1, 1, tight_layout=True)
        ax3.hist(momentumStorage.dropna(), bins=6)
        ax3.set_xlabel('Momentum storage [Nmsec]')
        ax3.set_ylabel('Count')
        plt.grid()
        plt.close(closePlot)
        fig3.savefig(saveFigPath + 'momentum_distribution.eps')

        # Draw Volume distribution plot
        volume          = metadata['volume']
        fig4, ax4       = plt.subplots(1, 1, tight_layout=True)
        ax4.hist(volume.dropna(), bins=n_bins)
        ax4.set_xlabel('Volume [m3]')
        ax4.set_ylabel('Count')
        plt.grid()
        plt.ticklabel_format(axis='x', style='sci', scilimits=None)    
        plt.close(closePlot)
        fig4.savefig(saveFigPath + 'volume_distribution.eps')

        # Draw Volume distribution plot
        power          = metadata['maxPower']
        fig5, ax5      = plt.subplots(1, 1, tight_layout=True)
        ax5.hist(power.dropna(), bins=n_bins)
        ax5.set_xlabel('Peak power [W]')
        ax5.set_ylabel('Count')
        plt.grid()
        plt.ticklabel_format(axis='x', style='sci', scilimits=None)    
        plt.close(closePlot)
        fig5.savefig(saveFigPath + 'power_distribution.eps')

elif typeOfPlot == "general": 
        ## Draw the general frequency of parameters given in the datasheet plot. 
        fig, axs         = plt.subplots( 1, 1, tight_layout=True )
        tempMetadata     = pd.notnull(metadata)
        attributeNames   = np.array([])
        ytickLocation    = np.array([])

        patches          = []
        x  = 0.0 
        y  = 0.5
        wide = 1.0

        ## Draw a reactangle with the width of the rectanglre representing the count of the attributes. 
        for column in tempMetadata:
            attributeValues     = np.array(tempMetadata[column])
            attributeNames      = np.append(attributeNames, column)
            attributeCount      = np.count_nonzero(attributeValues)   
            ytickLocation       = np.append( ytickLocation, y + (wide / 2.0)  ) 
            rect                = mpatches.Rectangle(xy=(x,y), width=attributeCount, height=wide)
            patches.append(rect)    
            y           = 2.0 + y
            pass 
        
        collection = mcollections.PatchCollection(patches, cmap=plt.cm.hsv, alpha=0.6)
        colors = np.linspace(0, 1, len(patches))
        collection.set_array(np.array(colors))
        axs.add_collection(collection)
        axs.autoscale()
        axs.set_yticks(ytickLocation)
        axs.set_yticklabels(attributeNames)
        fig.savefig(saveFigPath + "numberOfAttributesInDatasheets.eps")
        plt.close(False)

        ## Draw the different suppliers used for the data. 
        fig, axs                = plt.subplots( 1, 1, tight_layout=True )
        supplierNames           = ['MSC', 'BCT', 'HAI', 'GSP', 'SIP', 'RCD', 'NAN', 'BST', 'AUF']
        numberOfHardware        = [3.0, 7.0, 1.0, 1.0, 5.0, 9.0, 1.0, 1.0, 4.0]
        labelName               = ["Microsat Systems Canada", "Blue Canyon Technologies Inc", "Honeywell Aerospace Inc",        "Gomspace", "Sinclair Interplanetary", "Rockwell Collins Deutschland", "NanoAvionika Llc", "Berlin Space       Technologies GmbH", "Astro- und Feinwerktechnik Adlershof GmbH"]
        ytickLocation           = np.array([])
        # attributeCount   = np.array([])
        patches          = []
        x  = 0.0 
        y  = 0.5
        wide = 1.0

        for ii in range(len(supplierNames)):
            ytickLocation       = np.append( ytickLocation, y + (wide / 2.0)  ) 
            rect                = mpatches.Rectangle(xy=(x,y), width=numberOfHardware[ii], height=wide, 
                                        label=labelName[ii])
        #     print(labelName[ii])
            axs.legend(labelName[ii])
            patches.append(rect)    
            y           = 2.0 + y
            pass 

        collection = mcollections.PatchCollection(patches, cmap=plt.cm.hsv, alpha=0.6)
        colors = np.linspace(0, 1, len(patches))
        collection.set_array(np.array(colors))
        axs.add_collection(collection)
        # handles, labels = axs.get_legend_handles_labels()
        # print("Labels:", labels)
        # axs.legend(handles, labels)
        axs.autoscale()
        axs.set_yticks(ytickLocation)
        axs.set_yticklabels(supplierNames)
        plt.close()
        fig.savefig(saveFigPath + "supplierProducts.eps")
        # axs.legend()

# # Draw moments of inertia distribution plot.      

elif typeOfPlot == "scatter": 

        # Log plot of torque vs mass 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        sc = ax.scatter(mass, torque)
        ax.set_ylabel('Torque [Nm]')
        ax.set_xlabel('Mass [kg]')
        indexesSc     = names[~torque.isnull()]
        hoverFunctionality( ax, indexesSc, sc, fig, mass )

        ## Point out the anomalies in the graphs. rw3-0.060, rw-0.03
        y       = torque['RW3-0.060']
        x       = mass['RW3-0.060']
        Wide    = 0.25*x
        Height  = 0.25*y
        lowerX  = x - Wide/2.0 
        lowerY  = y - Height/2.0
        patch = mpatches.Rectangle((lowerX,lowerY), width=Wide, height=Height, edgecolor='r', facecolor='none', linewidth=2.0, label='RW3-0.060')
        ax.add_patch(patch)
        plt.legend(handles=[patch])

        y       = torque['RW-0.03']
        x       = mass['RW-0.03']
        Wide    = 0.25*x
        Height  = 0.25*y
        lowerX  = x - Wide/2.0 
        lowerY  = y - Height/2.0
        patch = mpatches.Rectangle((lowerX,lowerY), width=Wide, height=Height, edgecolor='r', facecolor='none', linewidth=2.0, label='RW-0.03')
        ax.add_patch(patch)
        plt.legend(handles=[patch])

        # sc2 = ax.scatter( mass, maxTorque*0.8, marker='*' )
        # sc3 = ax.scatter( mass, maxTorque, marker='*' )
        # sc3 = ax.scatter( mass, maxTorque*0.7, marker='*' )
        # sc3 = ax.scatter( mass, maxTorque*0.9, marker='*' )
        # sc3 = ax.scatter( mass, maxTorque*0.5, marker='*' )

        # indexesSc2      = names[~maxTorque.isnull()]
        # hoverFunctionality( ax, indexesSc2, sc2, fig, mass )
        # Customize the major grid
        # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'torqueVsMass.eps')

        # Log plot of torque vs volume 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        sc = ax.scatter(volume, torque)
        ax.set_ylabel('Torque [Nm]')
        ax.set_xlabel('Volume [m3]')
        indexesSc     = names[~torque.isnull()]
        hoverFunctionality( ax, indexesSc, sc, fig, volume )
        
        ## Point out the anomalies in the graphs. rw3-0.060, rw-0.03
        y       = torque['RW3-0.060']
        x       = volume['RW3-0.060']
        Wide    = 0.25*x
        Height  = 0.25*y
        lowerX  = x - Wide/2.0 
        lowerY  = y - Height/2.0
        patch = mpatches.Rectangle((lowerX,lowerY), width=Wide, height=Height, edgecolor='r', facecolor='none', linewidth=2.0, label='RW3-0.060')
        ax.add_patch(patch)
        plt.legend(handles=[patch])

        y       = torque['RW-0.03']
        x       = volume['RW-0.03']
        Wide    = 0.25*x
        Height  = 0.25*y
        lowerX  = x - Wide/2.0 
        lowerY  = y - Height/2.0
        patch = mpatches.Rectangle((lowerX,lowerY), width=Wide, height=Height, edgecolor='r', facecolor='none', linewidth=2.0, label='RW-0.03')
        ax.add_patch(patch)
        plt.legend(handles=[patch])

        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'torqueVsVolume.eps')


        # Log plot of torque vs mass 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        sc = ax.scatter(mass, momentum)
        ax.set_ylabel('Momentum storage [Nms]')
        ax.set_xlabel('Mass [kg]')
        indexesSc     = names[~momentum.isnull()]
        hoverFunctionality( ax, indexesSc, sc, fig, volume )
        # Customize the major grid
        # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'momentumVsMass.eps')


        # Log plot of volume vs momentum 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        sc = ax.scatter(volume, momentum)
        ax.set_ylabel('Momentum storage [Nms]')
        ax.set_xlabel('Volume [m3]')
        indexesSc     = names[~momentum.isnull()]
        hoverFunctionality( ax, indexesSc, sc, fig, volume )
        # Customize the major grid
        # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'momentumVsVolume.eps')

         # Log plot of torque vs mass 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        sc = ax.scatter(mass, peakPower)
        ax.set_ylabel('Power [W]')
        ax.set_xlabel('Mass [kg]')
        indexesSc     = names[~peakPower.isnull()]
        hoverFunctionality( ax, indexesSc, sc, fig, mass )
        ## Outliers: RW3-0.060, RWP050, RWP100, RW4, RW8
        y       = np.array([peakPower.loc['RW3-0.060'], peakPower.loc['RWP050'], peakPower.loc['RWP100'], peakPower.loc['RW4'], peakPower.loc['RW8'] ])
        x       = np.array([mass.loc['RW3-0.060'], mass.loc['RWP050'], mass.loc['RWP100'], mass.loc['RW4'], mass.loc['RW8'] ])
        labels  = [ 'RW3-0.060', 'RWP050', 'RWP100', 'RW4', 'RW8' ]
        
        for ii, jj, zz in zip(x, y, labels): 
                tempX   = ii
                tempY   = jj
                Wide    = 0.25*tempX
                Height  = 0.25*tempY
                lowerX  = tempX - Wide/2.0 
                lowerY  = tempY - Height/2.0
                patch = mpatches.Rectangle((lowerX,lowerY), width=Wide, height=Height, edgecolor='r', facecolor='none', linewidth=2.0, label=zz)
                ax.add_patch(patch)
                pass
        plt.legend(handles=[patch])

        # Customize the major grid
        # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'powerVsMass.eps')

        # Log plot of power vs volume 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        sc = ax.scatter(volume, peakPower)
        ax.set_ylabel('Power [W]')
        ax.set_xlabel('Volume [m3]')
        indexesSc     = names[~peakPower.isnull()]
        hoverFunctionality( ax, indexesSc, sc, fig, volume )
        # Customize the major grid
        # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'powerVsVolume.eps')

        # Log plot of Torque vs Momentum storage. 
        fig5, ax5       = plt.subplots(1, 1, tight_layout=True ) 
        ax5.set_xscale( "log" )
        ax5.set_yscale( "log" )
        sc  = ax5.scatter( momentum, torque, marker='.')
        indexesSc     = names[~torque.isnull()]
        hoverFunctionality( ax5, indexesSc, sc, fig5, momentum )
        # sc2 = ax5.scatter( momentum, maxTorque, marker='*' )
        # indexesSc2      = names[~maxTorque.isnull()]
        # hoverFunctionality( ax5, indexesSc2, sc2, fig5, momentum )
        # ## Outliers: RW3-0.060, RWP050, RWP100, RW4, RW8
        # y       = np.array([peakPower.loc['RW3-0.060'], peakPower.loc['RWP050'], peakPower.loc['RWP100'], peakPower.loc['RW4'], peakPower.loc['RW8'] ])
        # x       = np.array([mass.loc['RW3-0.060'], mass.loc['RWP050'], mass.loc['RWP100'], mass.loc['RW4'], mass.loc['RW8'] ])
        # labels  = [ 'RW3-0.060', 'RWP050', 'RWP100', 'RW4', 'RW8' ]
        # for ii, jj, zz in zip(x, y, labels): 
        #         tempX   = ii
        #         tempY   = jj
        #         Wide    = 0.25*tempX
        #         Height  = 0.25*tempY
        #         lowerX  = tempX - Wide/2.0 
        #         lowerY  = tempY - Height/2.0
        #         patch = mpatches.Rectangle((lowerX,lowerY), width=Wide, height=Height, edgecolor='r', facecolor='none', linewidth=2.0, label=zz)
        #         ax.add_patch(patch)
        #         pass
        plt.legend(handles=[patch])
        ax5.set_xlabel('Momentum Storage [Nms]')
        ax5.set_ylabel('Torque [Nm]')
        # ax5.set_title('Momentum Storage vs Torque for reaction wheels')
        # Customize the major grid
        ax5.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax5.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig5.savefig(saveFigPath + 'torqueVsMomentum.eps')

        # Log plot of Torque vs Momentum storage. 
        fig5, ax5       = plt.subplots(1, 1, tight_layout=True ) 
        ax5.set_xscale( "log" )
        ax5.set_yscale( "log" )
        torquePowerRatio     = ( torque / peakPower )
        sc  = ax5.scatter( momentum, torquePowerRatio, marker='.')
        indexesSc     = names[~torquePowerRatio.isnull()]
        hoverFunctionality( ax5, indexesSc, sc, fig5, momentum )
        # sc2 = ax5.scatter( momentum, maxTorque, marker='*' )
        # indexesSc2      = names[~maxTorque.isnull()]
        # hoverFunctionality( ax5, indexesSc2, sc2, fig5, momentum )
        ax5.set_xlabel('Momentum Storage [Nms]')
        ax5.set_ylabel('Torque/Power Ratio [Nm/W]')
        # plt.yticks((10^-3, 10^-2))
        # ax5.set_title('Momentum Storage vs Torque for reaction wheels')
        # Customize the major grid
        ax5.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax5.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig5.savefig(saveFigPath + 'torquePowerRatioVsMomentum.eps')

        # Log plot of momentum storage/mass vs angular momentum 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.scatter(momentum, momentum/(mass))
        ax.set_xlabel('Momentum storage [Nms]')
        ax.set_ylabel('Momentum storage / mass [Nms/kg]')
        # ax.set_title('Momentum storage per unit mass vs Momentum storage')
        # Customize the major grid
        # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'momentumVsMomentumPerMass.eps')

        # Log plot of momentum storage/mass vs angular momentum 
        fig, ax     = plt.subplots(1, 1, tight_layout=True)
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.scatter(momentum, momentum/(volume))
        ax.set_xlabel('Momentum storage [Nms]')
        ax.set_ylabel('Momentum storage / Volume [Nms/m3]')
        # ax.set_title('Momentum storage per unit mass vs Momentum storage')
        # Customize the major grid
        # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # Customize the minor grid
        ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        plt.close(closePlot)
        fig.savefig(saveFigPath + 'momentumVsMomentumPerVolume.eps')


        # # Log plot of Torque vs Momentum storage. 
        # fig5, ax5       = plt.subplots(1, 1, tight_layout=True ) 
        # ax5.set_xscale( "log" )
        # ax5.set_yscale( "log" )

        # sc = ax5.scatter( momentum, torque)
        # hoverFunctionality( ax5, names, sc, fig5, momentum )

        # sc2 = ax5.scatter( maxMomentum, torque)
        # hoverFunctionality( ax5, names, sc2, fig5, maxMomentum )

        # ax5.set_xlabel('Momentum Storage [Nms]')
        # ax5.set_ylabel('Max torque [Nm]')
        # ax5.set_title('Momentum Storage vs Max torque for reaction wheels')
        # # Customize the major grid
        # ax5.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # # Customize the minor grid
        # ax5.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        # plt.close()
        # fig5.savefig(saveFigPath + 'torqueVsMomentum.eps')

        # # # Log plot of torque vs peakPower 
        # fig, ax     = plt.subplots(1, 1, tight_layout=True)
        # ax.set_xscale("log")
        # ax.set_yscale("log")
        # ax.scatter(peakPower, torque)
        # ax.scatter(peakPower, maxTorque, marker='*' )
        # # patch = patches.Circle( (1.0,0.007), radius=5.0)
        # # patch = mpatches.Rectangle([0.8,0.006], width = 0.4, height=0.002, edgecolor='r', facecolor='none', linewidth=2.0, label='Anomaly')
        # # ax.add_patch(patch)
        # # calculated = ax.scatter(8.5281, 0.007, marker='*', color='red', label='Calculated Value', linewidth=2.0)
        # # ax.legend(['Calculated Value'])

        # ax.set_xlabel('Peak Power [W]')
        # ax.set_ylabel('Torque [Nm]')
        # # ax.set_title('Peak Power vs Torque')
        # # Customize the major grid
        # # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # # Customize the minor grid
        # ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        # # plt.legend(handles=[patch, calculated])

        # plt.close()
        # # fig.savefig(saveFigPath + 'torqueVspeakPower.eps')

        # # # Log plot of torque vs peakPower 
        # fig, ax     = plt.subplots(1, 1, tight_layout=True)
        # ax.set_xscale("log")
        # ax.set_yscale("log")
        # ax.scatter(peakPower, momentum)
        # ax.scatter(peakPower, maxMomentum, marker='*' )
        # ax.set_xlabel('Peak Power [W]')
        # ax.set_ylabel('Momentum storage [Nms]')
        # # ax.set_title('Peak Power vs Momentum storage')
        # # Customize the major grid
        # # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # # Customize the minor grid
        # ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        # plt.close()
        # fig.savefig(saveFigPath + 'momentumStorageVspeakPower.eps')

        # # Log plot of torque vs angular momentum/mass 
        # fig, ax     = plt.subplots(1, 1, tight_layout=True)
        # ax.set_xscale("log")
        # ax.set_yscale("log")
        # ax.scatter(momentumStorage/(mass), torque)
        # ax.set_xlabel('Momentum storage / mass [Nms/kg]')
        # ax.set_ylabel('Torque [Nm]')
        # # ax.set_title('Momentum storage per unit mass vs Torque')
        # # Customize the major grid
        # # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
        # # Customize the minor grid
        # ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
        # plt.close(showplot)
        # fig.savefig(saveFigPath + 'torqueVsMomentumPerMass.eps')
        
        pass 
# # Log plot of momentum storage/mass vs angular momentum 
# fig, ax     = plt.subplots(1, 1, tight_layout=True)
# ax.set_xscale("log")
# ax.set_yscale("log")
# ax.scatter(momentumStorage, momentumStorage/(volume))
# ax.set_xlabel('Momentum storage [Nms]')
# ax.set_ylabel('Momentum storage / volume [Nms/m3]')
# # ax.set_title('Momentum storage per unit volume vs Momentum storage')
# # Customize the major grid
# # ax.grid(which='major', linestyle='-', linewidth='0.5', color='red')
# # Customize the minor grid
# ax.grid(which='minor', linestyle='--', linewidth='0.5', color='black')
# plt.close(showplot)
# fig.savefig(saveFigPath + 'momentumVsMomentumPerVolume.eps')

# Violin plots Mass distributions: 
# dataset     = [ [metadata['mass']], [metadata['torque']], [metadata['momentumStorage']]  ]
# dataset     = pd.DataFrame( np.array( metadata['mass'], metadata['torque'] ), columns=['mass', 'torque'] )

# fig, ax     = plt.subplots(1, 1, tight_layout=True)
# attributeNames      = ["mass"]
# attributeValues     = [mass, torque, momentumStorage]
# ax                  = sns.violinplot(dataset) 
# ax.set_yscale("log")

# Show plots 
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