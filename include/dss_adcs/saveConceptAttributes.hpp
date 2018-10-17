/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_SAVE_CONCEPT_ATTRIBUTES_HPP
#define DSS_ADCS_SAVE_CONCEPT_ATTRIBUTES_HPP

#include <numeric> // std::accumulate

#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{

static bool abs_compare(double a, double b)
{
    return (std::abs(a) < std::abs(b));
}

inline const double abs_vector( double a, double b)
{
    const double retValue = std::abs(a) + std::abs(b); 
    return retValue; 
}

//! Structure to save state histories of power profiles, motor torques, attitudes, 
//! attitude rates etc. 

struct SaveStateHistories
{
public: 
    VectorXd    reactionWheelPowerProfiles; 
    VectorXd    reactionWheelMotorTorques; 
    Vector3     attitudeRates; 
    Real        slewRate; 
    VectorXd    reactionWheelAngularMomentums; 
    Vector4     quaternion; 
    Real        systemPeakPower; 
};

typedef std::vector< SaveStateHistories > StateHistoryStorageContainer; 

class SaveHighLevelAttributes   
{
public: 
    SaveHighLevelAttributes( StateHistoryStorageContainer aStateHistoryStorageContainer )
        : stateHistoryStorageContainer( aStateHistoryStorageContainer ) 
    { }

    //! Get peak values of reaction wheel power and system peak power
    /*  getPeakPower() function of the class to extract the peak values of reaction wheel power 
     *  and peak system power from the state histories. 
     *  
     *  This is to write high-level parameters to the metadata file. Iterates over the time 
     *  history of power to extract peak power values of reaction wheel and total system. 
     *  
     *  @param[out]   std::pair< const Real, const VectorXd >  peakPower 
    */ 
    std::tuple< const Real, const VectorXd, const VectorXd > getPeakPower( 
                                    const std::vector<ReactionWheel> reactionWheelConcept )
    {
        std::vector< VectorXd > reactionWheelPowerHistories;  
        std::vector< double > peakSystemPowerHistories;  

        // Save the parameters for the entire time history 
        for( std::vector<SaveStateHistories>::iterator it = stateHistoryStorageContainer.begin(); it != stateHistoryStorageContainer.end(); ++it)
        {
            const SaveStateHistories tempStateHistories = *it; 
            reactionWheelPowerHistories.push_back( tempStateHistories.reactionWheelPowerProfiles );
            peakSystemPowerHistories.push_back( tempStateHistories.systemPeakPower);  
        }

        // System peak power
        std::vector<Real>::iterator peakSystemPowerDistance; 
        peakSystemPowerDistance     = std::max_element( peakSystemPowerHistories.begin(), 
                                        peakSystemPowerHistories.end(), abs_compare );  

        const Real peakSystemPower  = peakSystemPowerHistories[std::distance(peakSystemPowerHistories.begin(), peakSystemPowerDistance )]; 

        // Reaction wheel peak power
        std::vector<Real>::iterator reactionWheelPeakPowerDistance; 
        VectorXd reactionWheelPeakPower( reactionWheelPowerHistories[0].size() ); 
        
        // Peak power to given peak power percentage. 
        VectorXd reactionWheelPeakPowerPercentage( reactionWheelPowerHistories[0].size() ); 
        
        // Iterate over the number of reaction wheels. 
        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheelPowerHistories[0].size(); ++reactionWheelIterator )
        {
            std::vector<Real> reactionWheelTempPower;

            // Iterate over the time histories to save the power profiles for each reaction wheel. 
            for( std::vector<VectorXd>::iterator powerTimeHistoryIterator = reactionWheelPowerHistories.begin(); powerTimeHistoryIterator != reactionWheelPowerHistories.end(); ++powerTimeHistoryIterator )
            {
                VectorXd tempPowerContainer = *powerTimeHistoryIterator;
                reactionWheelTempPower.push_back( tempPowerContainer[reactionWheelIterator] ); 
            } 
            reactionWheelPeakPowerDistance = std::max_element( reactionWheelTempPower.begin(), reactionWheelTempPower.end(), abs_compare ); 
            
            //! Peak power of the reaction wheel in the simulation. 
            reactionWheelPeakPower[reactionWheelIterator] = reactionWheelTempPower[std::distance(reactionWheelTempPower.begin(), reactionWheelPeakPowerDistance )]; 
            
            //! Percentage of peak power in the simulation with respect to the peak power given for the hardware 
            //! in the datasheet. 
            const double peakPowerReactionWheelGiven    = reactionWheelConcept[reactionWheelIterator].peakPower; 
            reactionWheelPeakPowerPercentage[reactionWheelIterator] = ( reactionWheelPeakPower[reactionWheelIterator] /
                                                                        peakPowerReactionWheelGiven ) * 100.0; 
        }

        std::tuple< const Real, const VectorXd, const VectorXd > outputPower( peakSystemPower, reactionWheelPeakPower, reactionWheelPeakPowerPercentage );  
        
        return outputPower; 
    } 
    //! Get reaction wheel angular momentums. 
    /*  The operator getAngularMomentums is used to get peak and average angular momentums of all the reaction wheels. 
     *  from the time histories of angular momentums stored in the storageContainer. Iterate over the entire array   
     *  to find the peak values for each of the reaction wheels. 
     *  
     *  @param[out]   std::pair< const VectorXd, const VectorXd > outputAngularMomentums
    */
    std::tuple< const VectorXd, const VectorXd, const VectorXd > getAngularMomentums( 
                                                                const std::vector<ReactionWheel> reactionWheelConcept )
    {
        std::vector< VectorXd > reactionWheelMomentumHistories; 

        // Save the parameters for the entire time history 
        for( std::vector<SaveStateHistories>::iterator it = stateHistoryStorageContainer.begin(); it != stateHistoryStorageContainer.end(); ++it)
        {
            const SaveStateHistories tempStateHistories = *it; 
            reactionWheelMomentumHistories.push_back( tempStateHistories.reactionWheelAngularMomentums );
        }

        // Reaction wheel peak and average momentum. 
        std::vector<Real>::iterator reactionWheelPeakMomentumDistance; 
        VectorXd reactionWheelPeakMomentum( reactionWheelMomentumHistories[0].size() ); 
        VectorXd reactionWheelAverageMomentum( reactionWheelMomentumHistories[0].size() ); 
        VectorXd reactionWheelMomentumPercentage( reactionWheelMomentumHistories[0].size() ); 

        // Iterate over the number of reaction wheels. 
        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheelMomentumHistories[0].size(); ++reactionWheelIterator )
        {
            std::vector<Real> reactionWheelTempMomentum;

            // Iterate over the time histories to save the power profiles for each reaction wheel. 
            for( std::vector<VectorXd>::iterator momentumTimeHistoryIterator = reactionWheelMomentumHistories.begin(); momentumTimeHistoryIterator != reactionWheelMomentumHistories.end(); ++momentumTimeHistoryIterator )
            { 
                VectorXd tempMomentumContainer = *momentumTimeHistoryIterator;
                reactionWheelTempMomentum.push_back( tempMomentumContainer[reactionWheelIterator] );  
            } 
            reactionWheelPeakMomentumDistance = std::max_element( reactionWheelTempMomentum.begin(), reactionWheelTempMomentum.end(), abs_compare ); 
            
            //! Calculate the peal and average momentum for each reaction wheel. 
            reactionWheelPeakMomentum[reactionWheelIterator] = reactionWheelTempMomentum[std::distance(reactionWheelTempMomentum.begin(), reactionWheelPeakMomentumDistance )]; 
            reactionWheelAverageMomentum[reactionWheelIterator] = ( std::accumulate( reactionWheelTempMomentum.begin(), 
                                                                    reactionWheelTempMomentum.end(), 0.0, abs_vector ) ) / reactionWheelTempMomentum.size();  

            //! Percentage of peak power in the simulation with respect to the peak power given for the hardware 
            //! in the datasheet. 
            const double peakMomentumReactionWheelGiven    = reactionWheelConcept[reactionWheelIterator].maxMomentumStorage; 
            reactionWheelMomentumPercentage[reactionWheelIterator] = std::abs( ( reactionWheelPeakMomentum[reactionWheelIterator] /
                                                                        peakMomentumReactionWheelGiven ) * 100.0 );
            
        }
        std::tuple< const VectorXd, const VectorXd, const VectorXd > outputMomentum( reactionWheelPeakMomentum, 
                                                    reactionWheelAverageMomentum, reactionWheelMomentumPercentage );  
        return outputMomentum; 
    }

private: 
    //! State history storage container.
    StateHistoryStorageContainer stateHistoryStorageContainer; 

protected: 

}; 

}

#endif 