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
    tolerance = 1e-3
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

# Calculate the percentage of mommentum storage w.r.t to the total momentum storage. 
def calculateSaturationPercentage( metadataPath, stateHistoryFiles ): 
    
    metadata               = pd.read_csv( metadataPath )
    maximumMomentumStorage = np.array([ metadata['maxMomentumStorage1'], metadata['maxMomentumStorage2'], 
                                        metadata['maxMomentumStorage3'], metadata['maxMomentumStorage4'] ])
    peakMomentumStoragePercent  = np.empty((0,4))
    # avgMomentumStoragePercent   = []
    for jj in range( len(stateHistoryFiles) ): 
        oneReactionWheelPercent     = np.array([])
        stateHistory                = pd.read_csv( stateHistoryFiles[jj] )
        stateHistoryMomentumStorage = np.array( [ stateHistory['angularMomentum1'], stateHistory['angularMomentum2'],
                                                  stateHistory['angularMomentum3'], stateHistory['angularMomentum4'] ] ) 
        # print("Peak momentums: ", np.amax(np.abs(stateHistoryMomentumStorage[0])), np.amax(np.abs(stateHistoryMomentumStorage[1])), 
        #                                     np.amax(np.abs(stateHistoryMomentumStorage[2])), np.amax(np.abs(stateHistoryMomentumStorage[3])) )
        for ii in range(len( maximumMomentumStorage ) ):  
            momentumStoragePercent          = ( stateHistoryMomentumStorage[ii] / maximumMomentumStorage[ii, jj] ) * 100.0
            # print(momentumStoragePercent.shape)
            tempPeakMomentumStoragePercent  = np.amax( np.abs(momentumStoragePercent) )
            # print("Peak momentum temp: ", tempPeakMomentumStoragePercent)
            # maxPowerReactionWheel      = np.append( maxPowerReactionWheel, np.amax( tempMaxPower ) )
            oneReactionWheelPercent      = np.append( oneReactionWheelPercent, tempPeakMomentumStoragePercent)
            pass 
        # print(oneReactionWheelPercent)
        peakMomentumStoragePercent = np.append( peakMomentumStoragePercent, [oneReactionWheelPercent], axis=0 )
        pass 
    pass 
    return peakMomentumStoragePercent

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
    maxPowerReactionWheels = np.empty((0,4))
    avgPowerReactionWheels = np.empty((0,4))

    for ii in range(len(stateHistoryFiles)):
        
        stateHistoryFile    = stateHistoryFiles[ii]
        stateHistory        = pd.read_csv(stateHistoryFile)
        powerStrings        = stringLocator( stateHistoryFile, 'powerConsumption')

        maxPowerReactionWheel        = np.array([])
        avgPowerReactionWheel        = np.array([])
        # reactionWheelPowerProfiles    = np.empty([])
        reactionWheelPowerSum         = stateHistory['totalSystemPower']

        for jj in range(len(powerStrings)):
            # reactionWheelPowerProfiles  = np.append(reactionWheelPowerProfiles, np.abs(stateHistory[powerStrings[jj]]))
            tempMaxPower                = np.abs(stateHistory[powerStrings[jj]])

            avgPowerReactionWheel      = np.append( avgPowerReactionWheel, np.average( tempMaxPower ) ) 
            maxPowerReactionWheel      = np.append( maxPowerReactionWheel, np.amax( tempMaxPower ) )
            pass 
        # print(avgPowerReactionWheel)
        # print(maxPowerReactionWheel)
        # reactionWheelPowerSum = np.sum( reactionWheelPowerProfiles, axis=0 )
        maxPower        = np.append( maxPower, np.amax( reactionWheelPowerSum ) )    
        avgPower        = np.append( avgPower, np.average( reactionWheelPowerSum ) )
        maxPowerReactionWheels = np.append( maxPowerReactionWheels, [maxPowerReactionWheel], axis=0 )
        # print(maxPowerReactionWheels)
        avgPowerReactionWheels = np.append( avgPowerReactionWheels, [avgPowerReactionWheel], axis=0 )
        # print(maxPowerReactionWheels)
        pass 
    # print("Update the power model to extract single reaction wheels power models.")
    # print(maxPower)
    return maxPower, avgPower, maxPowerReactionWheels, avgPowerReactionWheels    

def extractConceptAttributes( stateHistoryFiles ):

    maxReactionWheelTorque  = np.empty((0,4))
    avgReactionWheelTorque  = np.empty((0,4))

    maxReactionWheelAngularMomentum  = np.empty((0,4))
    avgReactionWheelAngularMomentum  = np.empty((0,4))

    maxReactionWheelAngularVelocity  = np.empty((0,4))
    avgReactionWheelAngularVelocity  = np.empty((0,4))

    for ii in range(len(stateHistoryFiles)): 
        stateHistoryFile        = stateHistoryFiles[ii]
        stateHistory            = pd.read_csv(stateHistoryFile)
        torqueStrings           = stringLocator( stateHistoryFile, 'motorTorque')
        angularMomentumStrings  = stringLocator( stateHistoryFile, 'angularMomentum')
        angularVelocityStrings  = stringLocator( stateHistoryFile, 'reactionWheelAngularVelocity')

        maxMotorTorque        = np.array([])
        avgMotorTorque        = np.array([])
        
        maxAngularMomentum        = np.array([])
        avgAngularMomentum        = np.array([])
        
        maxAngularVelocity        = np.array([])
        avgAngularVelocity        = np.array([])

        for jj in range(len(torqueStrings)):

            tempMotorTorque       = np.abs(stateHistory[torqueStrings[jj]])
            tempAngularMomentum   = np.abs(stateHistory[angularMomentumStrings[jj]])
            tempAngularVelocity   = np.abs(stateHistory[angularVelocityStrings[jj]])

            avgMotorTorque      = np.append( avgMotorTorque, np.average( tempMotorTorque ) ) 
            maxMotorTorque      = np.append( maxMotorTorque, np.amax( tempMotorTorque ) )
            avgAngularMomentum  = np.append( avgAngularMomentum, np.average( tempAngularMomentum ) ) 
            maxAngularMomentum  = np.append( maxAngularMomentum, np.amax( tempAngularMomentum ) )
            avgAngularVelocity  = np.append( avgAngularVelocity, np.average( tempAngularVelocity ) ) 
            maxAngularVelocity  = np.append( maxAngularVelocity, np.amax( tempAngularVelocity ) )

            pass 

        # Save maximum and average motor torque of each reaction wheel from each state history files.  
        maxReactionWheelTorque  = np.append( maxReactionWheelTorque, [maxMotorTorque], axis=0 )
        avgReactionWheelTorque  = np.append( avgReactionWheelTorque, [avgMotorTorque], axis=0 )
        
        # Save maximum and average angular momentum of each reaction wheel from each state history files. 
        maxReactionWheelAngularMomentum  = np.append( maxReactionWheelAngularMomentum, [maxAngularMomentum], axis=0 )
        avgReactionWheelAngularMomentum  = np.append( avgReactionWheelAngularMomentum, [avgAngularMomentum], axis=0 )
        
        # Save maximum and average angular momentum of each reaction wheel from each state history files. 
        maxReactionWheelAngularVelocity  = np.append( maxReactionWheelAngularVelocity, [maxAngularVelocity], axis=0 )
        avgReactionWheelAngularVelocity  = np.append( avgReactionWheelAngularVelocity, [avgAngularVelocity], axis=0 )
        pass 
    pass

    return maxReactionWheelTorque, avgReactionWheelTorque, maxReactionWheelAngularMomentum, avgReactionWheelAngularMomentum, maxReactionWheelAngularVelocity, avgReactionWheelAngularVelocity

# def calculateCalculateToGivenPercentage( calculatedValue, givenValue ): 
    
#     percent       = np.array([])
#     for ii in range(len(calculatedValue)):
#         tempPercent        = ( calculatedValue[ii] / givenValue[ii] ) * 100.0 
#         percent   = np.append(percent, tempPercent) 
#         pass 
#     return percent
    # pass
 
# Calculate the percentage of mommentum storage w.r.t to the total momentum storage. 
def calculatePeakPowerPercentage( metadataPath, stateHistoryFiles ): 
    
    metadata               = pd.read_csv( metadataPath )
    peakPowerStrings       = stringLocator( metadataPath, 'peakPower' )
    peakPower              = np.empty((0,4))
    peakPowerPercent       = np.array([])

    for jj in range(len(peakPowerStrings)): 
        peakPower = np.append( peakPower, metadata[peakPowerStrings[jj] ] )
        
        xx, yy, reactionWheelPeakPowerCalculated, zz    = calculatePower( stateHistoryFiles )
    # print("Peak Power: ", reactionWheelPeakPowerCalculated)
    # print("Datasheet: ", peakPower.shape)
    # print("Calculated: ", reactionWheelPeakPowerCalculated.shape)
        for ii in range(len( peakPower ) ):  
            peakPowerPercent          = np.append( peakPowerPercent, ( reactionWheelPeakPowerCalculated[jj,ii] / peakPower[ii] ) * 100.0 )
            pass 
        pass 
    return peakPowerPercent

# Calculate the percentage of mommentum storage w.r.t to the total momentum storage. 
# def calculateMomentumStoragePercentage( metadataPath, stateHistoryFiles ): 
    
#     metadata               = pd.read_csv( metadataPath )
#     peakPowerStrings       = stringLocator( metadataPath, 'angularMomentum' )
#     peakPower              = np.empty((0,4))
#     peakPowerPercent       = np.array([])

#     for jj in range(len(peakPowerStrings)): 
#         peakPower = np.append( peakPower, metadata[peakPowerStrings[jj] ] )
#         pass
#     xx, yy, reactionWheelPeakPowerCalculated, zz    = calculatePower( stateHistoryFiles )
#     print("Datasheet: ", peakPower.shape)
#     print("Calculated: ", reactionWheelPeakPowerCalculated.shape)
#     for ii in range(len( peakPower ) ):  
#         peakPowerPercent          = np.append( peakPowerPercent, ( reactionWheelPeakPowerCalculated[:,ii] / peakPower[ii] ) * 100.0 )
#         pass 
#     pass 
#     return peakPowerPercent

if __name__ == "__main__":
    filePath = "/home/superman/Desktop/single_simulation/state_history.csv"
    state_history = pd.read_csv( filePath )

    import re
    from csv_functions import requiredFiles

    def natural_sort(l): 
        convert = lambda text: int(text) if text.isdigit() else text.lower() 
        alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)', key) ] 
        return sorted(l, key = alphanum_key)

    settlingTimeFinal = calculateSettlingTime( filePath )
    # print( "The settling time is: ", settlingTimeFinal )

    referenceAttitude       = [0, 0, 0, 1]
    angularVelocityGain     = 2 * 0.707 * 0.1 
    slewRate                = np.deg2rad(0.2)

    saturationRate, startOfCoastingPhase, endOfCoastingPhase = calculateSaturationTime( filePath, referenceAttitude, angularVelocityGain, slewRate ) 

    # print("Time during saturation: ", saturationRate, startOfCoastingPhase, endOfCoastingPhase )

    bulkFilePath         = "/home/superman/Desktop/bulk_simulation"
    # stateHistoryFiles    = csv_functions.requiredFiles( bulkFilePath, 'state_history', '4.csv') 
    stateHistoryFiles       = np.array([filePath])
    print( "State history files: ", stateHistoryFiles )
    maxPower, avgPower, maxPowerReactionWheels, avgPowerReactionWheels  = calculatePower(stateHistoryFiles)
    print("Peak power system: ", maxPower )
    print("Peak power RW: ", maxPowerReactionWheels )

    # # Calculate the saturation % from the state history files. 
    # metadataFilePath    = "/home/superman/Desktop/metadata.csv"
    # path                = "/home/superman/Desktop/monte_carlo_single"
    # stringToSearchWith          = 'state_history'
    # filesForTheplots            = natural_sort( requiredFiles( path, stringToSearchWith ) )

    # testPeakMomentumStoragePercent = calculateSaturationPercentage( metadataFilePath, filesForTheplots )
    # print(testPeakMomentumStoragePercent.shape)
    # # Calculate reaction wheel max and average torques. 
    # # maxReactionWheelTorque, avgReactionWheelTorque, maxMomentum, avgMomentum, maxVelocity, avgVelocity  = extractConceptAttributes( stateHistoryFiles )
    
    # # print(maxReactionWheelTorque)
    # # print(avgReactionWheelTorque)
    # # print(maxMomentum)
    # # print(avgMomentum)
    # # print(maxVelocity)
    # # print(avgVelocity)

    # # # Calculate reaction wheel peak power percent. 
    # # reactionWheelPeakPowerGiven      = [ 50.0, 10.0, 1.0, 100.0] # Reaction wheel power given (hardware)
    # # reactionWheelPeakPowerCalculated = [ 40.0, 1.0, 5.0, 10.0]

    # # reactionWheelPowerPercent       = calculateCalculateToGivenPercentage( reactionWheelPeakPowerCalculated, reactionWheelPeakPowerGiven)
    # # print(reactionWheelPowerPercent)