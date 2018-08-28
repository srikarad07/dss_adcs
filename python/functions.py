"""
Copyright (c) 2017,S.D Cherukuri, Delft University of Technology (srikarad007@gmail.com)
Distributed under the MIT License.
See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
"""

import pandas as pd
import numpy as np 

import csv_functions
from csv_functions import stringLocator

def calculateSettlingTime( filePath ):

    # State history from the filePath 
    state_history = pd.read_csv( filePath ) 

    # settlingTimeIdx     = [ idx for i in range(slew_rate_history.si ]
    tolerance = 1e-10
    slew_rate_history = np.array( state_history['slewRate'] )
    time_history  = np.array( state_history['t'] / 60.0 )
    
    # print("Calculating settling time ... ")
    
    for i in range( slew_rate_history.size ): 

        if ( i < slew_rate_history.size - 12 ):        
            a = np.absolute(slew_rate_history[i] - slew_rate_history[i+2])
            b = np.absolute(slew_rate_history[i+2] - slew_rate_history[i+4])
            c = np.absolute(slew_rate_history[i+4] - slew_rate_history[i+6])
            d = np.absolute(slew_rate_history[i+8] - slew_rate_history[i+10])
            e = np.absolute(slew_rate_history[i+10] - slew_rate_history[i+12])

            if ( a < tolerance and b < tolerance and c < tolerance and d < tolerance and e < tolerance and np.absolute(slew_rate_history[i]) < tolerance ): 
                settlingTime        = time_history[i] 
                break 
            elif( i == slew_rate_history.size - 13):
                print("Settling time doesn't exist. Need more simulation time for file: ", filePath)
                settlingTime = None  
                pass 
            pass 

        pass
    return settlingTime

def calculateSaturationTime( filePath, reference_attitude, angular_velocity_gain, slew_saturation_rate ): 
    
    # State history from the filePath 
    state_history = pd.read_csv( filePath ) 

    # Start of the coasting phase calculated with formula from Bong wie (EigenAxis rotation with slew rate saturation). 
    startOfCoastingPhase    =  np.ceil( 4.0 / angular_velocity_gain )

    # Compute the commanded attitude matrix for quaternion error equation. 
    commandedAttitudeMatrix = np.array([[ reference_attitude[3], reference_attitude[2], -reference_attitude[1], -reference_attitude[0]], 
                                         [-reference_attitude[2], reference_attitude[3], reference_attitude[0], -reference_attitude[1]], 
                                         [ reference_attitude[1], -reference_attitude[0], reference_attitude[3], -reference_attitude[2]], 
                                         [reference_attitude[0], reference_attitude[1], reference_attitude[2], reference_attitude[3]] ])
    
    # Extract attitude states from the state_history file. 
    q1                      = np.array( state_history['q1'])
    q2                      = np.array( state_history['q2'])
    q3                      = np.array( state_history['q3'])
    q4                      = np.array( state_history['q4'])

    # Extract the reaction wheel velocities. 
    stringToBeLocated           = 'reactionWheelAngularVelocity'
    reactionWheelVelocityNames  =  stringLocator( filePath, stringToBeLocated )

    # print(startOfCoastingPhase)
    initialAttitude         = np.array([ q1[0], q2[0], q3[0], q4[0]])
    initialAttitudeError    = commandedAttitudeMatrix.dot(initialAttitude)
    initialAttitudeErrorNorm = np.linalg.norm(initialAttitudeError)

    directionQuaternionAtStartOfCoastingPhase  = q4[startOfCoastingPhase.astype(int)]
    
    endOfCoastingPhase = startOfCoastingPhase + ( 2 / slew_saturation_rate ) * np.arctan( initialAttitudeErrorNorm / directionQuaternionAtStartOfCoastingPhase ) 

    saturationRate = ( endOfCoastingPhase - startOfCoastingPhase ) / 60.0
    
    return saturationRate, startOfCoastingPhase/60.0, endOfCoastingPhase/60.0

def meanNormalization( vectorX ):
    
    averagedVector      = np.average( vectorX )
    denominator         = np.amax(vectorX) - np.amin(vectorX) 
    # normalizedVector    = [ vector for vector in range(len(vectorX)) ] 
    normalizedVector    = np.array([ ])
    for ii in range(len(vectorX)): 
        tempNormalizedVector    = ( vectorX[ii] - averagedVector ) / denominator  
        normalizedVector        = np.append( normalizedVector, tempNormalizedVector )
        pass 
    return normalizedVector

def calculatePerformance( settlingTime, saturationTime ): 
    
    normalizedSettlingTime      = meanNormalization( settlingTime )
    normalizedSaturationTime    = meanNormalization( saturationTime )
    # print("Saturation time: ", len(normalizedSaturationTime) )
    # print("Saturation time: ", len(normalizedSettlingTime) )
    performanceReverse          = normalizedSaturationTime + normalizedSettlingTime 

    return performanceReverse

def calculateCost( mass, volume, avgPower ): 
    
    normalizedMass              = meanNormalization( mass )
    normalizedVolume            = meanNormalization( volume )
    normalizedAvgPower          = meanNormalization( avgPower )

    cost                        = normalizedMass + normalizedVolume + normalizedAvgPower 

    return cost  

def calculatePower( stateHistoryFiles ):
    maxPower    = np.array([])
    avgPower    = np.array([])
    for ii in range(len(stateHistoryFiles)):
        
        stateHistoryFile    = stateHistoryFiles[ii]
        stateHistory        = pd.read_csv(stateHistoryFile)
        powerStrings        = stringLocator( stateHistoryFile, 'powerConsumption')
        
        maxPowerReactionWheels        = np.array([])
        avgPowerReactionWheels        = np.array([])

        for jj in range(len(powerStrings)):
            tempMaxPower            = stateHistory[powerStrings[jj]]
            avgPowerReactionWheels  = np.append( avgPowerReactionWheels, np.average( tempMaxPower ) ) 
            maxPowerReactionWheels  = np.append( maxPowerReactionWheels, np.amax( tempMaxPower ) )
            pass 
        
        maxPower        = np.append( maxPower, np.sum( maxPowerReactionWheels ) )    
        avgPower        = np.append( avgPower, np.sum( avgPowerReactionWheels ) )
        # print(maxPowerReactionWheels)
        pass 
    # print(maxPower)
    return maxPower, avgPower    

filePath = "/home/superman/Desktop/single_simulation/state_history.csv"
state_history = pd.read_csv( filePath )

settlingTimeFinal = calculateSettlingTime( filePath )
print( "The settling time is: ", settlingTimeFinal )

referenceAttitude       = [0, 0, 0, 1]
angularVelocityGain     = 2 * 0.707 * 0.1 
slewRate                = np.deg2rad(0.2)

saturationRate, startOfCoastingPhase, endOfCoastingPhase = calculateSaturationTime( filePath, referenceAttitude, angularVelocityGain, slewRate ) 

print("Time during saturation: ", saturationRate, startOfCoastingPhase, endOfCoastingPhase )

bulkFilePath         = "/home/superman/Desktop/bulk_simulation"
# stateHistoryFiles    = csv_functions.requiredFiles( bulkFilePath, 'state_history', '4.csv') 
stateHistoryFiles       = np.array([filePath])
maxPower, avgPower             = calculatePower(stateHistoryFiles)

print("The maximum power is: ", maxPower)
print("The average power is: ", avgPower)

