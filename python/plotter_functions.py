'''
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
'''

import numpy as np 
import matplotlib
import matplotlib.pyplot as plt

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

def plotWithSystemConstraintsAndRequirements( systemRequirements, systemConstraints, xAxisParameterToPlot, yAxisParameterToPlot, ax, state_history ):
    
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
    return sc 

