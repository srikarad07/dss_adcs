/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#include <cmath>

#include <catch.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#include "dss_adcs/outputWriter.hpp"
#include "dss_adcs/saveConceptAttributes.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{
namespace tests 
{

//! Test the stateHistoryWriterTemp with the stateHistoryWriterFunction! 
/* The state history writer consists of two classes: 'stateHistoryWriter' & 
 * 'stateHistoryWriterToInternalStructure'. Both the classes are tested by 
 *  using dummy variables in a loop to the state history writer and checking  
 *  if the same parameters are written to the file by both the functions. 
*/

TEST_CASE("Test Case 1: Test the stateHistoryWriterTemp with stateHistoryWriter")
{
    //! Tolerance 
    const Real tolerance = 1.0e-6;
    
    //! Create input parameters for the state history writer. 
    //! Output stream to write state history to.
    std::ofstream stateHistoryFile( "temp/temp/temp" );
    //! Input state 
    VectorXd inputState(11,1);
    inputState << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0; 
    
    VectorXdIntegration inputStateForIntegration( inputState.data(), inputState.data() + inputState.rows() * inputState.cols() );

    const double time = 1.0;
    //! Store state history storage container. 
    StateHistoryStorageContainer stateHistoryStorageContainer; 
    //! Control torque 
    std::vector< Vector3 > controlTorque; 
    const Vector3 controlTorque1( 20.0, 10.0, 10.0); controlTorque.push_back(controlTorque1);  
    const Vector3 controlTorque2( 10.0, 15.0, 12.0); controlTorque.push_back(controlTorque2);
    const Vector3 controlTorque3( 22.0, 13.0, 13.0); controlTorque.push_back(controlTorque3);
    const Vector3 controlTorque4( 21.0, 1.0, 5.0); controlTorque.push_back(controlTorque4);

    //! Motor torques for 4 reaction wheels for 4 epochs 
    std::vector< VectorXd > motorTorque;     
    VectorXd motorTorque1(4,1); motorTorque1 << 20.0, 10.0, -25.0, -35.0; motorTorque.push_back(motorTorque1); 
    motorTorque.push_back(motorTorque1);     
    VectorXd motorTorque2(4,1); motorTorque2 << 2.0, 3.0, -4.0, -10.0; motorTorque.push_back(motorTorque2); 
    motorTorque.push_back(motorTorque2);     
    VectorXd motorTorque3(4,1); motorTorque3 << 1.0, 2.0, -15.0, -25.0; motorTorque.push_back(motorTorque3);
    motorTorque.push_back(motorTorque3);     
    VectorXd motorTorque4(4,1); motorTorque4 << 22.0, 13.0, -35.0, -15.0; motorTorque.push_back(motorTorque4);
    motorTorque.push_back(motorTorque4);

    //! Disturbance Torque for 4 epochs
    std::vector< Vector3 > disturbanceTorque;
    const Vector3 disturbanceTorque1( 20.0, 10.0, 10.0); disturbanceTorque.push_back(disturbanceTorque1);  
    const Vector3 disturbanceTorque2( 10.0, 15.0, 12.0); disturbanceTorque.push_back(disturbanceTorque2);
    const Vector3 disturbanceTorque3( 22.0, 13.0, 13.0); disturbanceTorque.push_back(disturbanceTorque3);
    const Vector3 disturbanceTorque4( 21.0, 1.0, 5.0); disturbanceTorque.push_back(disturbanceTorque4);

    //! Reaction Wheel angular velocities for 4 reaction wheels for 4 epochs 
    std::vector< VectorXd > reactionWheelAngularVelocities;
    VectorXd reactionWheelAngularVelocities1(4,1), reactionWheelAngularVelocities2(4,1), reactionWheelAngularVelocities3(4,1), reactionWheelAngularVelocities4(4,1);
    reactionWheelAngularVelocities1 << 1000.0, 2000.0, 3000.0, 500.0; reactionWheelAngularVelocities.push_back(reactionWheelAngularVelocities1); 
    reactionWheelAngularVelocities2 << 100.0, 200.0, 300.0, 50.0; reactionWheelAngularVelocities.push_back(reactionWheelAngularVelocities2);   
    reactionWheelAngularVelocities3 << 10.0, 20.0, 30.0, 5.0; reactionWheelAngularVelocities.push_back(reactionWheelAngularVelocities3);  
    reactionWheelAngularVelocities4 << 100.0, 2000.0, 30.0, 5000.0; reactionWheelAngularVelocities.push_back(reactionWheelAngularVelocities4);  

    //! Reaction wheel power consumption for 4 reaction wheels for 4 epochs 
    std::vector< VectorXd > reactionWheelPowerConsumption;
    VectorXd reactionWheelPowerConsumption1(4,1), reactionWheelPowerConsumption2(4,1), reactionWheelPowerConsumption3(4,1), reactionWheelPowerConsumption4(4,1);
    reactionWheelPowerConsumption1 << 1.0, 10.0, 5.0, 6.0; reactionWheelPowerConsumption.push_back(reactionWheelPowerConsumption1);
    reactionWheelPowerConsumption2 << 11.0, 20.0, 15.0, 8.0; reactionWheelPowerConsumption.push_back(reactionWheelPowerConsumption2);
    reactionWheelPowerConsumption3 << 10.0, 1.0, 2.5, 7.7; reactionWheelPowerConsumption.push_back(reactionWheelPowerConsumption3); 
    reactionWheelPowerConsumption4 << 1.0, 10.0, 5.0, 6.0; reactionWheelPowerConsumption.push_back(reactionWheelPowerConsumption4);
    
    for (unsigned int timeHistoryIterator = 0; timeHistoryIterator < 4; ++timeHistoryIterator )
    {
        const VectorXd iteratorControlTorque = controlTorque[timeHistoryIterator]; 
        const VectorXd iteratorMotorTorque = motorTorque[timeHistoryIterator]; 
        const VectorXd iteratorDisturbanceTorque = disturbanceTorque[timeHistoryIterator]; 
        const VectorXd iteratorReactionWheelAngularVelocities = reactionWheelAngularVelocities[timeHistoryIterator]; 
        const VectorXd iteratorReactionWheelPowerConsumption = reactionWheelPowerConsumption[timeHistoryIterator]; 

        StateHistoryWriterToInternalStructure testWriter( stateHistoryFile, 
                                                          stateHistoryStorageContainer,
                                                          iteratorControlTorque,
                                                          iteratorMotorTorque,
                                                          iteratorDisturbanceTorque,
                                                          iteratorReactionWheelAngularVelocities,
                                                          iteratorReactionWheelPowerConsumption  ); 
        testWriter( inputStateForIntegration, time ); 
    } 

    //! Expected values: 
    Vector4 expectedQuaternion(1.0, 2.0, 3.0, 4.0);
    VectorXd expectedAngularMomentums( 4, 1 );
    expectedAngularMomentums << 8.0, 9.0, 10.0, 11.0; 

    unsigned int it = 0; 
    //! Test the stateHistoryStorageContainer if the correct values are stored. 
    for( std::vector< SaveStateHistories >::iterator storageContainerIterator = stateHistoryStorageContainer.begin(); storageContainerIterator != stateHistoryStorageContainer.end(); ++storageContainerIterator )
    {
        //! Expected values
        const VectorXd expectedMotorTorque          = motorTorque[it];
        const VectorXd expectedPowerProfiles        = reactionWheelPowerConsumption[it]; 

        SaveStateHistories tempStateHistory   = *storageContainerIterator; 
        
        const VectorXd tempMotorTorque        = tempStateHistory.reactionWheelMotorTorques; 
        const VectorXd tempPowerProfiles      = tempStateHistory.reactionWheelPowerProfiles; 
        const VectorXd tempAngularMomentums   = tempStateHistory.reactionWheelAngularMomentums; 
        const Vector4 tempQuaternion          = tempStateHistory.quaternion;
        
        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < 4; ++reactionWheelIterator)
        {         
            REQUIRE( tempQuaternion[reactionWheelIterator] == Approx( expectedQuaternion[reactionWheelIterator] ).epsilon(tolerance) ); 

            // REQUIRE( tempAngularMomentums[reactionWheelIterator] == Approx( expectedAngularMomentums[reactionWheelIterator] ).epsilon(tolerance) ); 

            REQUIRE( tempPowerProfiles[reactionWheelIterator] == Approx( expectedPowerProfiles[reactionWheelIterator] ).epsilon(tolerance) );  

            REQUIRE( tempMotorTorque[reactionWheelIterator] == Approx( expectedMotorTorque[reactionWheelIterator] ).epsilon(tolerance) );  

        }
        it += 1; 
    } 
     
} // test case 1

}  // tests 

} // dss_adcs