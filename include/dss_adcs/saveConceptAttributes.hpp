/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_SAVE_CONCEPT_ATTRIBUTES_HPP
#define DSS_ADCS_SAVE_CONCEPT_ATTRIBUTES_HPP

#include <numeric> // std::accumulate

#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/typedefs.hpp"

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
    Real        time;
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
     *  @param[out] std::pair< const Real, const VectorXd > peakPower
    */
    std::tuple< SystemPowerPair, ReactionWheelPowerPair, const VectorXd> getPeakPower( 
                                    const std::vector<ReactionWheel> reactionWheelConcept )
    {
        std::vector< VectorXd > reactionWheelPowerHistories;  
        std::vector< Real > totalSystemPowerHistories;  
        std::vector< Real > totalSystemPowerForAverage; 

        // Save the parameters for the entire time history 
        for( std::vector<SaveStateHistories>::iterator it = stateHistoryStorageContainer.begin(); it != stateHistoryStorageContainer.end(); ++it)
        {
            const SaveStateHistories tempStateHistories = *it; 
            reactionWheelPowerHistories.push_back( tempStateHistories.reactionWheelPowerProfiles );
            totalSystemPowerHistories.push_back( tempStateHistories.systemPeakPower );  
            // if ( tempStateHistories.systemPeakPower > 1e-3 )
            // {
               totalSystemPowerForAverage.push_back( tempStateHistories.systemPeakPower ); 
            // }
        } 

        // System peak power
        std::vector<Real>::iterator peakSystemPowerDistance; 
        peakSystemPowerDistance     = std::max_element( totalSystemPowerHistories.begin(), 
                                        totalSystemPowerHistories.end(), abs_compare );  

        const Real peakSystemPower  = totalSystemPowerHistories[std::distance(totalSystemPowerHistories.begin(), peakSystemPowerDistance )]; 

        // Reaction wheel average system power. 
        const Real avgSystemPower   = ( std::accumulate( totalSystemPowerForAverage.begin(), 
                                                         totalSystemPowerForAverage.end(),
                                                         0.0 ) / totalSystemPowerForAverage.size() );
         
        // Reaction wheel peak power
        std::vector<Real>::iterator reactionWheelPeakPowerDistance; 
        VectorXd reactionWheelPeakPower( reactionWheelPowerHistories[0].size() ); 
        
        // Peak power to given peak power percentage. 
        VectorXd reactionWheelPeakPowerPercentage( reactionWheelPowerHistories[0].size() ); 
        
        // Reaction wheel average power. 
        VectorXd reactionWheelAveragePower( reactionWheelPowerHistories[0].size() ); 

        // Iterate over the number of reaction wheels. 
        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheelPowerHistories[0].size(); ++reactionWheelIterator )
        {
            std::vector<Real> reactionWheelTempPower;

            std::vector<Real>reactionWheelPowerForAverage; 

            // Iterate over the time histories to save the power profiles for each reaction wheel. 
            for( std::vector<VectorXd>::iterator powerTimeHistoryIterator = reactionWheelPowerHistories.begin(); powerTimeHistoryIterator != reactionWheelPowerHistories.end(); ++powerTimeHistoryIterator )
            {
                VectorXd tempPowerContainer = *powerTimeHistoryIterator;
                reactionWheelTempPower.push_back( tempPowerContainer[reactionWheelIterator] ); 
                if ( tempPowerContainer[reactionWheelIterator] > 1e-3)
                {
                    reactionWheelPowerForAverage.push_back( tempPowerContainer[reactionWheelIterator] );
                }
            } 

            reactionWheelPeakPowerDistance = std::max_element( reactionWheelTempPower.begin(), reactionWheelTempPower.end(), abs_compare ); 

            //! Peak power of the reaction wheel in the simulation. 
            reactionWheelPeakPower[reactionWheelIterator] = reactionWheelTempPower[std::distance(reactionWheelTempPower.begin(), reactionWheelPeakPowerDistance )]; 
            
            //! Percentage of peak power in the simulation with respect to the peak power given for the hardware 
            //! in the datasheet. 
            const double peakPowerReactionWheelGiven    = reactionWheelConcept[reactionWheelIterator].peakPower; 
            reactionWheelPeakPowerPercentage[reactionWheelIterator] = ( 
                                                reactionWheelPeakPower[reactionWheelIterator] /
                                                        peakPowerReactionWheelGiven ) * 100.0; 

            //! Average power of reaction wheels in the simulation. 
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
            // The average power is taken for elements less than 1e-4. 
            // Need to verify if it is actually correct. 
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
            reactionWheelAveragePower[reactionWheelIterator] = ( std::accumulate( reactionWheelPowerForAverage.begin(), 
                                                            reactionWheelPowerForAverage.end(), 
                                                            0.0 ) / reactionWheelPowerForAverage.size() );  
            // reactionWheelAveragePower[reactionWheelIterator] = std::accumulate( reactionWheelTempPower.begin(), 
            //                                                 reactionWheelTempPower.end(), 
            //                                                 0.0 ) / reactionWheelTempPower.size() );
        }   
        SystemPowerPair peakAndAverageSystemPower( peakSystemPower, avgSystemPower ); 
        ReactionWheelPowerPair peakAndAveragePowerReactionWheelWheel( reactionWheelPeakPower, reactionWheelAveragePower );  
        std::tuple< SystemPowerPair, ReactionWheelPowerPair, const VectorXd > outputPower(                                                                               peakAndAverageSystemPower,
                                                              peakAndAveragePowerReactionWheelWheel,
                                                              reactionWheelPeakPowerPercentage );  
        
        return outputPower; 
    } 

    const std::tuple< const VectorXd, const VectorXd, const VectorXd > getReactionWheelTorques( const std::vector<ReactionWheel> reactionWheelConcept )
    {
        std::vector< VectorXd > reactionWheelTorqueHistories; 

        // Save the parameters for the entire time history 
        for( std::vector<SaveStateHistories>::iterator it = stateHistoryStorageContainer.begin(); it != stateHistoryStorageContainer.end(); ++it)
        {
            const SaveStateHistories tempStateHistories = *it; 
            reactionWheelTorqueHistories.push_back( tempStateHistories.reactionWheelMotorTorques );
            
        }

        // Reaction wheel peak and average momentum. 
        std::vector<Real>::iterator reactionWheelPeakTorqueDistance; 
        VectorXd reactionWheelPeakTorque( reactionWheelTorqueHistories[0].size() ); 
        VectorXd reactionWheelAverageTorque( reactionWheelTorqueHistories[0].size() ); 
        VectorXd reactionWheelPeakTorquePercentage( reactionWheelTorqueHistories[0].size() ); 
        VectorXd reactionWheelAverageTorquePercent( reactionWheelTorqueHistories[0].size() ); 

        // Iterate over the number of reaction wheels. 
        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheelTorqueHistories[0].size(); ++reactionWheelIterator )
        {
            std::vector<Real> reactionWheelTempTorque;
            std::vector<Real>reactionWheelTorqueForAverage; 

            // Iterate over the time histories to save the power profiles for each reaction wheel. 
            for( std::vector<VectorXd>::iterator torqueTimeHistoryIterator = reactionWheelTorqueHistories.begin(); torqueTimeHistoryIterator != reactionWheelTorqueHistories.end(); ++torqueTimeHistoryIterator )
            { 
                VectorXd tempTorqueContainer = *torqueTimeHistoryIterator;
                reactionWheelTempTorque.push_back( std::abs( tempTorqueContainer[reactionWheelIterator] ) );  
                if ( tempTorqueContainer[reactionWheelIterator] > 1e-5)
                {
                    reactionWheelTorqueForAverage.push_back( tempTorqueContainer[reactionWheelIterator] );
                }
            } 
            reactionWheelPeakTorqueDistance = std::max_element( reactionWheelTempTorque.begin(), reactionWheelTempTorque.end(), abs_compare ); 
            
            //! Calculate the peak and torque for each reaction wheel. 
            reactionWheelPeakTorque[reactionWheelIterator] = std::abs( reactionWheelTempTorque[std::distance(reactionWheelTempTorque.begin(), reactionWheelPeakTorqueDistance )] ); 
            
            //! Average torque of reaction wheels in the simulation. 
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
            // The average torque is taken for elements less than 1e-4. 
            // Need to verify if it is actually correct. 
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
            reactionWheelAverageTorque[reactionWheelIterator] = ( std::accumulate( reactionWheelTorqueForAverage.begin(), 
                                                            reactionWheelTorqueForAverage.end(), 
                                                            0.0 ) / reactionWheelTorqueForAverage.size() ); 
            // reactionWheelAverageTorque[reactionWheelIterator] = ( std::accumulate( reactionWheelTempTorque.begin(), 
            //                                                         reactionWheelTempTorque.end(), 0.0, abs_vector ) ) / reactionWheelTempTorque.size();  
            
            //! Percentage of peak power in the simulation with respect to the peak power given for the hardware 
            //! in the datasheet. 
            const double peakTorqueReactionWheelGiven    = reactionWheelConcept[reactionWheelIterator].maxTorque; 
            reactionWheelPeakTorquePercentage[reactionWheelIterator] = std::abs( ( reactionWheelPeakTorque[reactionWheelIterator] /
                                                                        peakTorqueReactionWheelGiven ) * 100.0 );
            
            reactionWheelAverageTorquePercent[reactionWheelIterator]   = std::abs( ( reactionWheelAverageTorque[reactionWheelIterator] / peakTorqueReactionWheelGiven ) * 100.0 );
            
        }
        std::tuple< const VectorXd, const VectorXd, const VectorXd > outputReactionWheelTorques(                                                                   reactionWheelPeakTorque, 
                                                        reactionWheelPeakTorquePercentage, 
                                                        reactionWheelAverageTorquePercent );  
        return outputReactionWheelTorques; 
    };

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
            std::vector<Real>reactionWheelMomentumForAverage; 

            // Iterate over the time histories to save the power profiles for each reaction wheel. 
            for( std::vector<VectorXd>::iterator momentumTimeHistoryIterator = reactionWheelMomentumHistories.begin(); momentumTimeHistoryIterator != reactionWheelMomentumHistories.end(); ++momentumTimeHistoryIterator )
            { 
                VectorXd tempMomentumContainer = *momentumTimeHistoryIterator;
                reactionWheelTempMomentum.push_back( std::abs( tempMomentumContainer[reactionWheelIterator]) );  
                // if ( tempMomentumContainer[reactionWheelIterator] > 1e-5)
                // {
                    reactionWheelMomentumForAverage.push_back( tempMomentumContainer[reactionWheelIterator] );
                // }
            } 
            reactionWheelPeakMomentumDistance = std::max_element( reactionWheelTempMomentum.begin(), reactionWheelTempMomentum.end(), abs_compare ); 
            
            //! Calculate the peal and average momentum for each reaction wheel. 
            reactionWheelPeakMomentum[reactionWheelIterator] = std::abs( reactionWheelTempMomentum[std::distance(reactionWheelTempMomentum.begin(), reactionWheelPeakMomentumDistance )] ); 
            reactionWheelAverageMomentum[reactionWheelIterator] = std::abs( ( std::accumulate( reactionWheelMomentumForAverage.begin(), 
                                                                    reactionWheelMomentumForAverage.end(), 0.0, abs_vector ) ) / reactionWheelMomentumForAverage.size() );  

            //! Percentage of peak power in the simulation with respect to the peak power given for the hardware 
            //! in the datasheet. 
            const double peakMomentumReactionWheelGiven    = reactionWheelConcept[reactionWheelIterator].maxMomentumStorage; 
            reactionWheelMomentumPercentage[reactionWheelIterator] = std::abs( ( reactionWheelPeakMomentum[reactionWheelIterator] /
                                                                        peakMomentumReactionWheelGiven ) * 100.0 );
            
        }
        std::tuple< const VectorXd, const VectorXd, const VectorXd > outputMomentum( reactionWheelPeakMomentum, 
                                                    reactionWheelAverageMomentum, reactionWheelMomentumPercentage );  
        return outputMomentum; 
    };

    //! Get the maneuver time for the attitude control concept/ 
    /*  Maneuver time is understood as the time during which the slew rate of the spacecraft  
     *  becomes constant.  
     */
    const Real calculateSettingTime( )
    {            
        //! Save time of the state histories. 
        std::vector<Real> stateHistoryTime; 

        //! Save the slew rate time histories. 
        std::vector<Real> stateHistorySlewRate; 

        //! Store the time and slew rate histories in the storage containers define above. 
        std::vector<SaveStateHistories>::iterator saveStateHistoryIterator; 
        for ( saveStateHistoryIterator = stateHistoryStorageContainer.begin(); saveStateHistoryIterator !=
              stateHistoryStorageContainer.end(); ++saveStateHistoryIterator )
        {
            const SaveStateHistories tempStateHistories = *saveStateHistoryIterator; 
            stateHistoryTime.push_back( tempStateHistories.time );
            stateHistorySlewRate.push_back( sml::convertRadiansToDegrees(tempStateHistories.slewRate) );  
        }

        //! Set the allowed tolerance for slew rate constancy. 
        const Real tolerance  = 1e-3; 

        //! Time iterator
        unsigned int timeIterator = 0; 

        //! Settling time storage container. 
        Real settlingTime( 0.0 ); 

        //! Loop over the slew rate histories to check the instant when it becomes constant. 
        for( unsigned int stateHistoryIterator = 0; stateHistoryIterator < stateHistorySlewRate.size(); 
             ++stateHistoryIterator )
        {
            if( stateHistoryIterator < (stateHistoryTime.size() - 12) )
            {
                const Real error1      = std::abs( stateHistorySlewRate[stateHistoryIterator] -
                                        stateHistorySlewRate[stateHistoryIterator + 4] );
                const Real error2      = std::abs( stateHistorySlewRate[stateHistoryIterator + 4] -
                                        stateHistorySlewRate[stateHistoryIterator + 6] );  
                const Real error3      = std::abs( stateHistorySlewRate[stateHistoryIterator + 6] -
                                        stateHistorySlewRate[stateHistoryIterator + 8] );  
                const Real error4      = std::abs( stateHistorySlewRate[stateHistoryIterator + 8] -
                                        stateHistorySlewRate[stateHistoryIterator + 10] );  
                const Real error5      = std::abs( stateHistorySlewRate[stateHistoryIterator + 10] -
                                        stateHistorySlewRate[stateHistoryIterator + 12] );  
                const Real error6      = std::abs( stateHistorySlewRate[stateHistoryIterator + 12] -
                                        stateHistorySlewRate[stateHistoryIterator + 14] );  
                // std::cout << "Tolerance: " << tolerance << " Errors: " << error1 << " , " << error2 << " , " << error3 << " , " << error4 << " , " << error5 << " , " << error6 << "," << (std::abs(stateHistorySlewRate[stateHistoryIterator] - 0)) << std::endl; 

                if( (error1 < tolerance) & (error2 < tolerance) & (error3 < tolerance) & 
                    (error4 < tolerance) & (error5 < tolerance) & (error6 < tolerance) & 
                    ( std::abs(stateHistorySlewRate[stateHistoryIterator] - 0) < tolerance) )
                {
                    settlingTime        = stateHistoryTime[stateHistoryIterator];  
                    break;
                }
                else if( (error1 > tolerance) & ( stateHistoryIterator == (stateHistoryTime.size() - 13) ) )
                {
                    settlingTime        = std::nan("SettlingTime");
                    // <<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
                    // Print out the concept information when the settling time is not found.
                    // <<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
                    std::cout << "The simulation needs more time to settle for the concept: " << std::endl; 
                    throw;
                }
            }

            ++timeIterator;
        } 

        //! Convert settling time from seconds to minutes. 
        return (settlingTime / 60.0); 
    };

private: 
    //! State history storage container.
    StateHistoryStorageContainer stateHistoryStorageContainer; 

protected: 

}; 

}

#endif 