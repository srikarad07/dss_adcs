'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

import numpy as np 
import matplotlib
import matplotlib.pyplot as plt

## Extract chunks of data from the array. 
def chunks(l, n):
    
    """Yield successive n-sized chunks from l."""
    for i in xrange(0, len(l), n):
        yield l[i:i + n]
    
    pass

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

    # print "The final concept values: ", requiredStateHistory[finalCriteria]
    
    ## Obtain the values that satisfy the constraints. 
    constrainedValues           = requiredStateHistory[finalCriteria] 

    # Get the index of values below constraints
    indexOfBelowConstraints             = constrainedValues.index.values
    
    ## Get the x axis and y axis parameters for the values below the the constraints. 
    constrainedXAxisParameterToPlot     = xParameterToPlot.loc[indexOfBelowConstraints]
    constrainedYAxisParameterToPlot     = yParameterToPlot.loc[indexOfBelowConstraints]

    return constrainedXAxisParameterToPlot, constrainedYAxisParameterToPlot

def hoverFunctionality( ax, names, sc, fig, xAxisParameterToPlot ): 
    
    c       = np.random.randint(1,5,size=len(xAxisParameterToPlot))
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

## Function to plot with system requirements and constraints. 
def plotWithSystemConstraintsAndRequirements( systemRequirements, systemConstraints, xAxisParameterToPlot, yAxisParameterToPlot, ax, state_history, plotProperties ):
    
    ## Check if the system requirements are defined by the user. 
    if  bool(systemRequirements) == 1 and bool(systemConstraints) != 1:
        sc =  ax.scatter( xAxisParameterToPlot, yAxisParameterToPlot, **plotProperties )

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
                            **plotProperties ) 
        
    elif bool(systemConstraints) == 1 and bool(systemRequirements) == 1: 
        
        ## Check for system constraints
        constrainedXAxisParameterToPlot, constrainedYAxisParameterToPlot = plotWithSystemLevelConstraints( xAxisParameterToPlot, 
                                        yAxisParameterToPlot, 
                                        state_history,
                                        systemConstraints, ax )
        sc =  ax.scatter( constrainedXAxisParameterToPlot,
                            constrainedYAxisParameterToPlot, 
                            **plotProperties ) 
        plotWithSystemRequirements( constrainedXAxisParameterToPlot, 
                                    constrainedYAxisParameterToPlot, 
                                    systemRequirements, ax )
        
    else: 
        sc =  ax.scatter( xAxisParameterToPlot, 
                            yAxisParameterToPlot, 
                           **plotProperties)
        pass 
    return sc 

def drawBoxplots( xAxisParameterToPlot, yAxisParameterToPlot, ax, state_history, numberOfSimulations ):
    
    tempXParameterToPlot    = np.array(xAxisParameterToPlot)
    
    ## Split the entire histories of parameters into chunks based on numberOfSimulation. 
    yParameterChunks        = chunks(yAxisParameterToPlot, numberOfSimulations)
    xParameterChunks        = chunks(tempXParameterToPlot, numberOfSimulations)

    yParameterBoxPlot       = []
    xParameterBoxPlot       = []

    for i, j in zip(yParameterChunks, xParameterChunks):     
        yParameterBoxPlot.append(i)
        xParameterBoxPlot.append(j)
        pass 

    ax.boxplot(yParameterBoxPlot, positions=np.average(xParameterBoxPlot, axis=1) ) 

    pass 

## Function to plot single simulation in either subplots or not. 

def singleSimulationPlots( subplotFlag, yAxisParameterString, xAxisParameterString, fig, state_history ): 
    
    if subplotFlag == True:
        jj = np.ceil( len(yAxisParameterString)/ 2.0 )
        for i in range(len(yAxisParameterString)):
            ax = fig.add_subplot(2,jj,i+1)
            ax.plot( state_history[xAxisParameterString], state_history[yAxisParameterString[i]] )
            # ax.set_title( 'Power' + str(i+1) + ' [W]' )
            # ax.set_xlabel('time[min]')
            handles, labels = ax.get_legend_handles_labels()
            # ax.legend(handles, labels)
            # ax.ticklabel_format(style='sci', axis='both', scilimits=(0,0))
            pass 
         
    elif subplotFlag == False: 
        for i in range(len(yAxisParameterString)):
            # ax = fig.add_subplot(2,jj,i+1)
            ax.plot( xAxisParameterString, state_history[yAxisParameterString[i]], color=colors[i],     linestyle=linestyles[i], linewidth=2.0 )
            # ax.set_xlabel('Time [min]')
            # ax.set_ylabel('Motor torque [N/m]')
            # ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
            pass 
    else: 
        print "Error!!!!! Subplotflag incorrectly defined!"
        exit()
        pass

    return ax 

def bulkSimulationPlots( yAxisParameterString, xAxisParameterString, xAxisParameterString2, state_history, metadata, ax, systemRequirements, systemConstraints, hoverFlag, names, fig, typeOfPlots, numberOfSimulations, plotProperties ): 
    
    for ii in range(len(yAxisParameterString)):
        ## Add the extra [] to get the result as a dataframe; it is used 
        ## later for a function! 
        yAxisParameterToPlot  = state_history[[yAxisParameterString[ii]]] 
        if xAxisParameterString.size == 0:
            xAxisParameterToPlot  = state_history[xAxisParameterString2] 
        else:    
            xAxisParameterToPlot  = metadata[xAxisParameterString] 
            pass 
        if typeOfPlots == "scatter": 
            ## Scatter plots with system requirements and constraints. 
            sc = plotWithSystemConstraintsAndRequirements( systemRequirements, systemConstraints, xAxisParameterToPlot, yAxisParameterToPlot, ax, state_history, plotProperties )
            
            ## Display concept identifier upon hovering over the plot.
            if hoverFlag:
                hoverFunctionality( ax, names, sc, fig, xAxisParameterToPlot ) 
                pass         
        
        elif typeOfPlots == "boxplot": 
             drawBoxplots( xAxisParameterToPlot, yAxisParameterToPlot, ax, state_history, numberOfSimulations )
        
        pass

        pass  

    pass # def bulkSimulationPlots 


