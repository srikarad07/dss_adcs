"""
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
"""

import re, csv
import os

def stringLocator(filePath, stringToBeLocated):
    with open(filePath, 'rb') as (f):
        reader = csv.reader(f, delimiter=' ', quotechar='|')
        your_list = map(tuple, reader)
        csvFirstRow = ('').join(your_list[0])
        string = stringToBeLocated + '\\d'
        textToSearch = re.compile(string)
        result = textToSearch.findall(csvFirstRow)
    return result

def requiredFiles( pathToDirectory, stringOfInterestingFiles, *moreKeywords ):
    
    if not moreKeywords: 
        interestingFiles    = [pathToDirectory + '/' + filename for filename in os.listdir(pathToDirectory) if os.path.isfile(pathToDirectory + '/' + filename) and stringOfInterestingFiles in filename]        
    elif len(moreKeywords) == 1: 
        interestingFiles    = [pathToDirectory + '/' + filename for filename in os.listdir(pathToDirectory) if os.path.isfile(pathToDirectory + '/' + filename) and stringOfInterestingFiles in filename and moreKeywords[0] in filename]
        pass 
    return interestingFiles

# path                =  "/home/superman/Desktop/single_simulation/state_history.csv"
# stringToBeLocated   = 'angularMomentum'

# stringLocator( path, stringToBeLocated )


