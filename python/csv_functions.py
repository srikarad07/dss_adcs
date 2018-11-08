"""
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
"""

import re, csv
import os
import numpy as np 
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

def stringLocator(filePath, stringToBeLocated):
    with open(filePath, 'rb') as (f):
        reader = csv.reader(f, delimiter=' ', quotechar='|')
        result = np.array([])
        your_list = map(tuple, reader)
        for ii in range(len(your_list)):

    ## Locate strings in python with regular expression '\d' represents 
    ## the numeric value after the given string
            csvFirstRow = ('').join(your_list[ii])
            string = stringToBeLocated 
            textToSearch = re.compile(string)
            result = np.append( result, textToSearch.findall(csvFirstRow) )
        pass 
    return result

def requiredFiles( pathToDirectory, stringOfInterestingFiles, *moreKeywords ):
    
    if not moreKeywords: 
        interestingFiles    = [pathToDirectory + '/' + filename for filename in os.listdir(pathToDirectory) if os.path.isfile(pathToDirectory + '/' + filename) and stringOfInterestingFiles in filename]        
    elif len(moreKeywords) == 1: 
        interestingFiles    = [pathToDirectory + '/' + filename for filename in os.listdir(pathToDirectory) if os.path.isfile(pathToDirectory + '/' + filename) and stringOfInterestingFiles in filename and moreKeywords[0] in filename]
        pass 
    return interestingFiles

## Names appearing after hovering over the point. 
def update_annot(ind, sc, annot):

    pos = sc.get_offsets()[ind["ind"][0]]
    annot.xy = pos
    text = "{}".format( " ".join([names[n] for n in ind["ind"]]))
    annot.set_text(text)
    annot.get_bbox_patch().set_facecolor(cmap(norm(c[ind["ind"][0]])))
    annot.get_bbox_patch().set_alpha(0.4)

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

if __name__ == "__main__":
    # path                = "/home/superman/Desktop/bulk_simulation/state_history_3.csv"
    # stringToBeLocated   = 'concept_3_*\\d\_\d\d'

    # results = stringLocator( path, stringToBeLocated )
    # print(results)

    x = np.random.rand(5)
    y = np.random.rand(5)
    names = np.array(["a", "b", "c", "d", "e"])
    c = np.random.randint(1,5,size=5)

    norm = plt.Normalize(1,4)
    cmap = plt.cm.RdYlGn

    fig,ax  = plt.subplots()
    sc      = plt.scatter(x,y,c=c, s=100, cmap=cmap, norm=norm)
    annot   = ax.annotate("", xy=(0,0), xytext=(20,20),textcoords="offset points",
                        bbox=dict(boxstyle="round", fc="w"),
                        arrowprops=dict(arrowstyle="->"))
    annot.set_visible(False)

    fig.canvas.mpl_connect("motion_notify_event", hover)
    plt.show()

