/*
 * Copyright (c) 2017, K. Kumar, Delft University of Technology (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_SAVE_CONCEPT_ATTRIBUTES_HPP
#define DSS_ADCS_SAVE_CONCEPT_ATTRIBUTES_HPP

namespace dss_adcs
{

static bool abs_compare(double a, double b)
{
    return (std::abs(a) < std::abs(b));
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
    std::pair< const Real, const VectorXd > getPeakPower( )
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
        
        // Iterate over the number of reaction wheels. 
        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheelPowerHistories[0].size(); ++reactionWheelIterator )
        {
            std::vector<Real> reactionWheelTempPower(reactionWheelPowerHistories.size());

            // Iterate over the time histories to save the power profiles for each reaction wheel. 
            for( std::vector<VectorXd>::iterator powerTimeHistoryIterator = reactionWheelPowerHistories.begin(); powerTimeHistoryIterator != reactionWheelPowerHistories.end(); ++powerTimeHistoryIterator )
            {
                VectorXd tempPowerContainer = *powerTimeHistoryIterator;
                reactionWheelTempPower.push_back( tempPowerContainer[reactionWheelIterator] ); 
            } 
            reactionWheelPeakPowerDistance = std::max_element( reactionWheelTempPower.begin(), reactionWheelTempPower.end(), abs_compare ); 
            
            reactionWheelPeakPower[reactionWheelIterator] = reactionWheelTempPower[std::distance(reactionWheelTempPower.begin(), reactionWheelPeakPowerDistance )]; 
        }

        std::pair< const Real, const VectorXd > outputPower( peakSystemPower, reactionWheelPeakPower );  
        
        return outputPower; 
    } 

private: 
    //! State history storage container.
    StateHistoryStorageContainer stateHistoryStorageContainer; 

protected: 

}; 

}

#endif 