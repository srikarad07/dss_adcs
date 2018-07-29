/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/numeric/odeint.hpp>
#include <boost/random/mersenne_twister.hpp>
// #include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#include <astro/astro.hpp>
#include <sml/sml.hpp>

#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/dynamicalSystem.hpp"
#include "dss_adcs/getReactionWheel.hpp"
#include "dss_adcs/objectiveFunction.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/monteCarloSingle.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/outputWriter.hpp"

namespace dss_adcs
{

void executeMonteCarloSingleSimulator( const rapidjson::Document& config )
{
    // Verify config parameters. Exception is thrown if any of the parameters are missing.
    const monteCarloSingleSimulatorInput input = checkMonteCarloSingleSimulatorInput( config );

    std::cout << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << "                           Run simulator                          " << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << std::endl;

    std::cout << "The API is being called to extract the parameters ... " << std::endl;
    std::vector< ReactionWheel > reactionWheels; 
    reactionWheels  = getReactionWheels( input.actuator, input.actuatorUuid );

    // Define the actuator configuration. 
    std::cout << "Defining actuator configuration ... \n" << std::endl;
     
    std::map< std::string, std::vector <ReactionWheel> > reactionWheelConcepts; 
    for ( unsigned int reactionWheelsIterator = 0; reactionWheelsIterator < reactionWheels.size(); ++reactionWheelsIterator )
    {
        ReactionWheel tempReactionWheel       = reactionWheels[reactionWheelsIterator];
        tempReactionWheel.wheelOrientation[0] = input.wheelOrientation[reactionWheelsIterator][0]; 
        tempReactionWheel.wheelOrientation[1] = input.wheelOrientation[reactionWheelsIterator][1];

        reactionWheelConcepts["Concept"].push_back( tempReactionWheel );   
    }
    
    const ActuatorConfiguration actuatorConfiguration( reactionWheelConcepts["Concept"] ); 

    // std::cout << "Reaction Wheel moment of inertia: " << actuatorConfiguration.calculateMomentOfInertia() << std::endl; 

    // Create instance of dynamical system.
    std::cout << "Setting up dynamical model ..." << std::endl;
    
    /*  testInt for the gravity gradient model 
    *   assumptions: radius is an assumption -> ideally radius should be derived from a ephemeris. 
    *   direction cosines and conversion from euler angles and quaternions. 
    *   assumption: random gravitational parameter defined for now -> ideally should be 
    *   taken from a reliable source. 
    */

    std::cout << "Setting up random number generators for principle inertia..." << std::endl;
    
    // Initialize random seed.
    boost::random::mt19937 randomSeed;

    // Random number generator for principle inertia vector. 
    boost::random::uniform_real_distribution< Real > principleInertiaGeneratorXX( input.principleInertiaMin[0], input.principleInertiaMax[0] );
    boost::random::uniform_real_distribution< Real > principleInertiaGeneratorYY( input.principleInertiaMin[1], input.principleInertiaMax[1] );
    boost::random::uniform_real_distribution< Real > principleInertiaGeneratorZZ( input.principleInertiaMin[2], input.principleInertiaMax[2] );

    // Random number generator for initial attitude state. 
    // boost::random::uniform_real_distribution< Real > initialAttitudeStateGeneratorQ1( input.initialAttitudeStateMin[0], input.initialAttitudeStateMax[0] );
    // boost::random::uniform_real_distribution< Real > initialAttitudeStateGeneratorQ2( input.initialAttitudeStateMin[1], input.initialAttitudeStateMax[1] );
    // boost::random::uniform_real_distribution< Real > initialAttitudeStateGeneratorQ3( input.initialAttitudeStateMin[2], input.initialAttitudeStateMax[2] );
    // boost::random::uniform_real_distribution< Real > initialAttitudeStateGeneratorQ4( input.initialAttitudeStateMax[3], input.initialAttitudeStateMin[3] );

    //Set up numerical integrator. 
    std::cout << "Executing numerical integrator ..." << std::endl;
    
    for ( int i = 0; i < input.numberOfSamples; ++i )
    {
        std::string stateHistoryFilePath = input.stateHistoryFilePath + '_' + std::to_string(i) + ".csv"; 
            
        // Create file stream to write state history to.
        std::ofstream stateHistoryFile( stateHistoryFilePath );
        if ( reactionWheels.size() == 3 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1,motorTorque2,motorTorque3,angularMomentum1,angularMomentum2,angularMomentum3,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2,reactionWheelAngularVelocity3,powerConsumption1,powerConsumption2,powerConsumption3,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        }
        else if ( reactionWheels.size() == 4 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1,motorTorque2,motorTorque3,motorTorque4,angularMomentum1,angularMomentum2,angularMomentum3,angularMomentum4,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2,reactionWheelAngularVelocity3,reactionWheelAngularVelocity4,powerConsumption1,powerConsumption2,powerConsumption3,powerConsumption4,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;

        }
        else if (  reactionWheels.size() == 5 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorquecontrolTorque3,motorTorque1,motorTorque2,motorTorque3,motorTorque4,angularMomentum1,angularMomentum2,angularMomentumangularMomentum4,angularMomentum5,reactionWheelAngularvelocity1,reactionWheelAngularvelocity2,reactionWheelAngularvelocityreactionWheelAngularVelocity4,reactionWheelAngularVelocity5,powerConsumption1,powerConsumption2,powerConsumptionpowerConsumption4,powerConsumption5,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        }
        else if (  reactionWheels.size() == 6 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorquecontrolTorque3,motorTorque1,motorTorque2,motorTorque3,motorTorque4,motorTorque5,motorTorque6,angularMomentum1,angularMomentumangularMomentum3,angularMomentum4,angularMomentum5,angularMomentum6,reactionWheelAngularVelocity1,reactionWheelAngularVelocityreactionWheelAngularVelocity3,reactionWheelAngularVelocity4,reactionWheelAngularVelocity5,reactionWheelAngularVelocitypowerConsumption1,powerConsumption2,powerConsumption3,powerConsumption4,powerConsumption5,powerConsumption6,disturbanceTorquedisturbanceTorque2,disturbanceTorque3" << std::endl;
        }
        else
        {
            std::cout << "The state history file not set up for " <<  reactionWheels.size() << " reaction wheels!" << std::endl; 
            throw; 
        }

        const Inertia principleInertia( principleInertiaGeneratorXX(randomSeed), principleInertiaGeneratorYY(randomSeed), principleInertiaGeneratorZZ(randomSeed) ) ;
        // std::cout << "Principle inertia: " << principleInertia << std::endl;
        // const State initialAttitudeState( input.initialAttitudeStateMin[0], input.initialAttitudeStateMin[1], 
        //                     input.initialAttitudeStateMin[1], input.initialAttitudeStateMin[3], 
        //                         input.initialAttitudeStateMin[4], input.initialAttitudeStateMin[5], input.initialAttitudeStateMin[6]  ); 

        // State initialAttitudeState;
        // initialAttitudeState[0] = initialAttitudeStateGeneratorQ1(randomSeed); 
        // initialAttitudeState[1] = initialAttitudeStateGeneratorQ2(randomSeed);  
        // initialAttitudeState[2] = initialAttitudeStateGeneratorQ3(randomSeed);  
        // initialAttitudeState[3] = initialAttitudeStateGeneratorQ4(randomSeed);  
        // initialAttitudeState[4] = input.initialAttitudeStateMin[4];  
        // initialAttitudeState[5] = input.initialAttitudeStateMin[5];
        // initialAttitudeState[6] = input.initialAttitudeStateMin[6]; 

        State initialAttitudeState;
        initialAttitudeState[0] = input.initialAttitudeStateMin[0];
        initialAttitudeState[1] = input.initialAttitudeStateMin[1];
        initialAttitudeState[2] = input.initialAttitudeStateMin[2];
        initialAttitudeState[3] = input.initialAttitudeStateMin[3];
        initialAttitudeState[4] = input.initialAttitudeStateMin[4];  
        initialAttitudeState[5] = input.initialAttitudeStateMin[5];
        initialAttitudeState[6] = input.initialAttitudeStateMin[6]; 

        VectorXd currentState( ( initialAttitudeState.size() + reactionWheelConcepts["Concept"].size() ) );                 
        for ( unsigned int stateIterator = 0; stateIterator < (initialAttitudeState.size() + reactionWheelConcepts["Concept"].size() ); ++stateIterator )
        {
            if ( stateIterator < initialAttitudeState.size() )
            {
                currentState[stateIterator] = initialAttitudeState[stateIterator]; 
            }
            else if ( stateIterator >= initialAttitudeState.size() && stateIterator < (initialAttitudeState.size() + reactionWheelConcepts["Concept"].size() ) )
            {
                currentState[stateIterator] = 0.0; 
            }
            else
            {
                std::cout << "Something is going wrong in the initial current state iterator! " << std::endl; 
            }
        }; 
        Vector4 referenceAttitudeState       = input.referenceAttitudeState; 

        // Set up dynamical model.
        std::cout << "Dynamical model setting up ..." << std::endl;

        // Set up dynamical model.
        std::cout << "Generating angular accelerations ..." << std::endl;
        std::cout << std::endl;

        std::vector< Vector4 > quaternionStateVector; 

        const Vector4 initialQuaternion(initialAttitudeState[0], initialAttitudeState[1], initialAttitudeState[2], initialAttitudeState[3]); 

        for ( Real integrationStartTime = input.startEpoch; integrationStartTime < input.endEpoch; integrationStartTime += input.timeStep )
        {
            Real integrationEndTime = integrationStartTime + input.timeStep; 

            Vector4 currentAttitude( currentState[0], currentState[1], currentState[2], currentState[3] ); 
            Vector3 currentAttitudeRate( currentState[4], currentState[5], currentState[6] ); 
            VectorXd reactionWheelAngularMomentums( reactionWheelConcepts["Concept"].size() ); 

            for ( unsigned int angularMomentumIterator = 0; angularMomentumIterator < reactionWheelAngularMomentums.size(); ++angularMomentumIterator)
            {
                reactionWheelAngularMomentums[angularMomentumIterator] = currentState[initialAttitudeState.size() + angularMomentumIterator ]; 
            }

            VectorXd reactionWheelAngularVelocities = actuatorConfiguration.computeReactionWheelVelocities(reactionWheelAngularMomentums);
            // std::cout << "Reaction wheel angular velocities" << reactionWheelAngularVelocities * 60 << std::endl; 

            // const Vector3 asymmetricBodyTorque    = astro::computeRotationalBodyAcceleration( principleInertia, currentAttitudeRate );
            const Vector3 asymmetricBodyTorque( 0.0, 0.0, 0.0 ); 

            // Disturbance torques. 
            Vector3 gravityGradientTorque( 0.0, 0.0, 0.0 ); 
            if ( input.gravityGradientAccelerationModelFlag != false )
            {
                gravityGradientTorque += astro::computeGravityGradientTorque( input.gravitationalParameter, input.radius, principleInertia, currentAttitude ); 
            }

            Vector3 disturbanceTorque( 0.0, 0.0, 0.0 );

            disturbanceTorque[0] += gravityGradientTorque[0];
            disturbanceTorque[1] += gravityGradientTorque[1];
            disturbanceTorque[2] += gravityGradientTorque[2]; 

            // Vector3 controlTorque( 0.0, 0.0, 0.0 ); 
            std::pair < Vector3, VectorXd > outputTorques = dss_adcs::computeRealTorqueValue( currentAttitude, 
                                                                                              referenceAttitudeState,
                                                                                              currentAttitudeRate, 
                                                                                              actuatorConfiguration, 
                                                                                              input.controllerType, 
                                                                                              input.naturalFrequency, 
                                                                                              input.dampingRatio, 
                                                                                              input.slewSaturationRate,
                                                                                              principleInertia, 
                                                                                              initialQuaternion, 
                                                                                              integrationStartTime ); 
            Vector3 controlTorque( outputTorques.first ); 
            VectorXd reactionWheelMotorTorque( outputTorques.second );  

            if ( input.controlTorqueActiveModelFlag == false )
            {
                controlTorque = { 0.0, 0.0, 0.0 };
            }

            StateHistoryWriter writer( stateHistoryFile, controlTorque, reactionWheelMotorTorque, disturbanceTorque, reactionWheelAngularVelocities );
    
            // Dynamics of the system 
            DynamicalSystem dynamics( asymmetricBodyTorque, controlTorque, disturbanceTorque, reactionWheelMotorTorque, principleInertia );

            // To Do: Use different integrator for the simulation. 
            if ( input.integrator == rk4 )
            {
                using namespace boost::numeric::odeint;
                integrate_const( runge_kutta4< VectorXd, double, VectorXd, double, vector_space_algebra >( ),
                                 dynamics,
                                 currentState,
                                 integrationStartTime,
                                 integrationEndTime,
                                 input.timeStep,
                                 writer );
            }
            else
            {
                std::cout << "Numerical integrator not defined" << std::endl;
                throw;
            }
            // quaternionStateVector.push_back( currentAttitude ); 
        }

    }
    // ObjectiveFunction objectiveFunction( reactionWheels, 
    //                                      quaternionStateVector, 
    //                                      input.stateHistoryFilePath );   
    // Real functionValue      = objectiveFunction.computeObjectiveFunction(); 

    // std::cout << "The objective function is: " << functionValue << std::endl; 
};

//! Check input parameters for the attitude_dynamics_simulator mode. 
monteCarloSingleSimulatorInput checkMonteCarloSingleSimulatorInput( const rapidjson::Document& config )
{
    // Extract principle inertia. 
    ConfigIterator principleInertiaMaxDiagonalIterator     = find( config, "principle_inertia_maximum"); 
    Inertia principleInertiaMax; 
    principleInertiaMax[0]                                 = principleInertiaMaxDiagonalIterator->value[0].GetDouble( ); 
    principleInertiaMax[1]                                 = principleInertiaMaxDiagonalIterator->value[1].GetDouble( ); 
    principleInertiaMax[2]                                 = principleInertiaMaxDiagonalIterator->value[2].GetDouble( );
    std::cout << "Max Principle inertia around X axis:                          " << principleInertiaMax[0]
              << "[kg/m^2]" << std::endl; 
    std::cout << "Max Principle inertia around Y axis:                          " << principleInertiaMax[1]
              << "[kg/m^2]" << std::endl;          
    std::cout << "Max Principle inertia around Z axis:                          " << principleInertiaMax[2]
              << "[kg/m^2]" << std::endl; 

    ConfigIterator principleInertiaMinDiagonalIterator     = find( config, "principle_inertia_minimum"); 
    Inertia principleInertiaMin; 
    principleInertiaMin[0]                                 = principleInertiaMinDiagonalIterator->value[0].GetDouble( ); 
    principleInertiaMin[1]                                 = principleInertiaMinDiagonalIterator->value[1].GetDouble( ); 
    principleInertiaMin[2]                                 = principleInertiaMinDiagonalIterator->value[2].GetDouble( );
    std::cout << "Min Principle inertia around X axis:                          " << principleInertiaMin[0]
              << "[kg/m^2]" << std::endl; 
    std::cout << "Min Principle inertia around Y axis:                          " << principleInertiaMin[1]
              << "[kg/m^2]" << std::endl;          
    std::cout << "Min Principle inertia around Z axis:                          " << principleInertiaMin[2]
              << "[kg/m^2]" << std::endl; 
    
    // Extract attitude kinematic type. 
    const std::string attitudeRepresentationString      = find( config, "attitude_representation" )->value.GetString( );
    std::cout << "Attitude representation:                                      " << attitudeRepresentationString << std::endl; 
    
    // Extract the initial attitude states and angular velocities. 
    ConfigIterator initialAttitudeStateEulerMinIterator    = find( config, "initial_attitude_state_minimum");
    ConfigIterator initialAttitudeStateEulerMaxIterator    = find( config, "initial_attitude_state_maximum");
    ConfigIterator referenceAttitudeStateIterator          = find( config, "attitude_reference_state" );
 
    Vector7 initialAttitudeStateMin, initialAttitudeStateMax;
    Vector4 referenceAttitudeState; 
    Vector3 initialAttitudeStateEulerMin, initialAttitudeStateEulerMax;

    if (attitudeRepresentationString.compare("euler_angles") == 0)
    {
        initialAttitudeStateEulerMin[0]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerMinIterator->value[0].GetDouble() );
        initialAttitudeStateEulerMin[1]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerMinIterator->value[1].GetDouble() );
        initialAttitudeStateEulerMin[2]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerMinIterator->value[2].GetDouble() );

        Vector4 quaternionInitialStateMin                 = astro::transformEulerToQuaternion( initialAttitudeStateEulerMin );
        initialAttitudeStateMin[0]                        = quaternionInitialStateMin[0];
        initialAttitudeStateMin[1]                        = quaternionInitialStateMin[1];
        initialAttitudeStateMin[2]                        = quaternionInitialStateMin[2];
        initialAttitudeStateMin[3]                        = quaternionInitialStateMin[3];   
        initialAttitudeStateMin[4]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMinIterator->value[3].GetDouble() ); 
        initialAttitudeStateMin[5]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMinIterator->value[4].GetDouble() ); 
        initialAttitudeStateMin[6]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMinIterator->value[5].GetDouble() ); 

        initialAttitudeStateEulerMax[0]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerMaxIterator->value[0].GetDouble() );
        initialAttitudeStateEulerMax[1]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerMaxIterator->value[1].GetDouble() );
        initialAttitudeStateEulerMax[2]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerMaxIterator->value[2].GetDouble() );

        Vector4 quaternionInitialStateMax                 = astro::transformEulerToQuaternion( initialAttitudeStateEulerMax );
        initialAttitudeStateMax[0]                        = quaternionInitialStateMax[0];
        initialAttitudeStateMax[1]                        = quaternionInitialStateMax[1];
        initialAttitudeStateMax[2]                        = quaternionInitialStateMax[2];
        initialAttitudeStateMax[3]                        = quaternionInitialStateMax[3];   
        initialAttitudeStateMax[4]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMaxIterator->value[3].GetDouble() ); 
        initialAttitudeStateMax[5]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMaxIterator->value[4].GetDouble() ); 
        initialAttitudeStateMax[6]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMaxIterator->value[5].GetDouble() ); 
        Vector3 referenceAttitudeStateEuler; 

        referenceAttitudeStateEuler[0]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[0].GetDouble() );
        referenceAttitudeStateEuler[1]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[1].GetDouble() );
        referenceAttitudeStateEuler[2]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[2].GetDouble() ); 

        referenceAttitudeState                         = astro::transformEulerToQuaternion( referenceAttitudeStateEuler );    
    }
    else if ( attitudeRepresentationString.compare("quaternions") == 0 )
    {
        initialAttitudeStateMin[0]                        = initialAttitudeStateEulerMinIterator->value[0].GetDouble();
        initialAttitudeStateMin[1]                        = initialAttitudeStateEulerMinIterator->value[1].GetDouble();
        initialAttitudeStateMin[2]                        = initialAttitudeStateEulerMinIterator->value[2].GetDouble();
        initialAttitudeStateMin[3]                        = initialAttitudeStateEulerMinIterator->value[3].GetDouble();   
        initialAttitudeStateMin[4]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMinIterator->value[4].GetDouble() ); 
        initialAttitudeStateMin[5]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMinIterator->value[5].GetDouble() ); 
        initialAttitudeStateMin[6]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMinIterator->value[6].GetDouble() ); 

        initialAttitudeStateMax[0]                        = initialAttitudeStateEulerMaxIterator->value[0].GetDouble();
        initialAttitudeStateMax[1]                        = initialAttitudeStateEulerMaxIterator->value[1].GetDouble();
        initialAttitudeStateMax[2]                        = initialAttitudeStateEulerMaxIterator->value[2].GetDouble();
        initialAttitudeStateMax[3]                        = initialAttitudeStateEulerMaxIterator->value[3].GetDouble();   
        initialAttitudeStateMax[4]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMaxIterator->value[4].GetDouble() ); 
        initialAttitudeStateMax[5]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMaxIterator->value[5].GetDouble() ); 
        initialAttitudeStateMax[6]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerMaxIterator->value[6].GetDouble() ); 

        referenceAttitudeState[0]                      = referenceAttitudeStateIterator->value[0].GetDouble();
        referenceAttitudeState[1]                      = referenceAttitudeStateIterator->value[1].GetDouble();
        referenceAttitudeState[2]                      = referenceAttitudeStateIterator->value[2].GetDouble();  
        referenceAttitudeState[3]                      = referenceAttitudeStateIterator->value[3].GetDouble();
    }
    else 
    {
        std::cout << "This attitude representation is not defined! Please enter the initial attitude state in accepted representations! " << std::endl; 
        throw; 
    }

    std::cout << "Max Initial attitude state in quaternion:                         " << initialAttitudeStateMax[0]<< "," << initialAttitudeStateMax[1]<< "," << initialAttitudeStateMax[2]<< "," << initialAttitudeStateMax[3] << "," << initialAttitudeStateMax[4] << "," << initialAttitudeStateMax[5] << "," << initialAttitudeStateMax[6] << std::endl;

    std::cout << "Min Initial attitude state in quaternion:                         " << initialAttitudeStateMin[0]<< "," << initialAttitudeStateMin[1]<< "," << initialAttitudeStateMin[2]<< "," << initialAttitudeStateMin[3] << "," << initialAttitudeStateMin[4] << "," << initialAttitudeStateMin[5] << "," << initialAttitudeStateMin[6] << std::endl;

    std::cout << "Reference state in quaternion:                                " << referenceAttitudeState[0]<< "," << referenceAttitudeState[1]<< "," << referenceAttitudeState[2]<< "," << referenceAttitudeState[3] << std::endl;  
    
    // Number of monte carlo samples. 
    const int numberOfSamples                  = find( config, "number_0f_samples")->value.GetInt( );  

    // Extract integrator type. 
    const std::string integratorString                  = find( config, "integrator" )->value.GetString( );
    Integrator integrator = rk4;
    if ( integratorString.compare( "rk4" ) != 0 )
    {
        if ( integratorString.compare( "rkf78" ) == 0 )
        {
            integrator = rkf78;
        }
        else if ( integratorString.compare( "dopri5" ) == 0 )
        {
            integrator = dopri5;
        }
        else if ( integratorString.compare( "bs" ) == 0 )
        {
            integrator = bs;
        }
        else if ( integratorString.compare( "testInt" ) == 0 )
        {
            integrator = testInt;
        }
        else
        {
            std::cout << std::endl;
            std::cerr << "Selected numerical integrator \""
                      << integratorString
                      << "\" is incorrect!" << std::endl;
            throw;
        }
    }
    std::cout << "Integrator                                                    " << integratorString << std::endl;

    // Extract integrator time setttings. 
    const Real startEpoch                              = find( config, "start_epoch")->value.GetDouble( );
    std::cout << "Intergration start epoch:                                     " << startEpoch 
              << "[sec]"        << std::endl; 
    const Real endEpoch                                = find( config, "end_epoch")->value.GetDouble( );
    std::cout << "Integration end epoch:                                        " << endEpoch 
              << "[sec]" << std::endl;
    const Real timeStep                                = find( config, "time_step" )->value.GetDouble( );
    std::cout << "Timestep of the integration is:                               " << timeStep 
              << "[sec]" << std::endl; 
    
    // Extract gravitational parameter of the central body.  
    const Real gravitationalParameter                 = find( config, "gravitational_parameter")->value.GetDouble(); 
    std::cout << "Gravitational Parameter:                                      " << gravitationalParameter
              << "[km^3 s^-2]" << std::endl; 

    // Extract the radial distance of the central body. 
    const Real radius                                 = find( config, "radius")->value.GetDouble();
    std::cout << "Radial vector:                                                "      << radius
              << "[km]" << std::endl; 

    // Exract the semi major axis of the orbit. 
    const Real semiMajorAxis                          = find( config, "semi_major_axis")->value.GetDouble(); 
    std::cout << "Semi major axis:                                              "   << semiMajorAxis
              << "[km]" << std::endl; 

    // Extract integrator tolerances.  
    const Real relativeTolerance                      = find( config, "relative_tolerance")->value.GetDouble(); 
    std::cout << "Relative Tolerance:                                           " << relativeTolerance
              << "[-]" << std::endl; 
    const Real absoluteTolerance                      = find( config, "absolute_tolerance")->value.GetDouble(); 
    std::cout << "Absolute Tolerance:                                           " << absoluteTolerance
              << "[-]" << std::endl;

    // Extract gravity gradient model .
    const bool gravityGradientAccelerationModelFlag = find( config, "is_gravity_gradient_active" )->value.GetBool( );
    std::cout << "Is Gravity Gradient disturbance model active?                 " << gravityGradientAccelerationModelFlag << std::endl;

    // Extract actuator model and parameters. 
    const std::string attitudeControlMethod     = find( config, "attitude_control_method")->value.GetString();
    std::cout << "Attitude control method:                                      " << attitudeControlMethod << std::endl; 
    
    // Extract the name of the actuator
    const std::string actuator                  = find( config, "actuator")->value.GetString(); 
    std::cout << "Actuator used for control:                                    " << actuator << std::endl; 

    // Extract reaction wheel attributes.
    std::vector < std::string > actuatorUuid; 
    std::vector< Vector2 > wheelOrientation; 
    
    const rapidjson::Value& reactionWheelsIterator 	= config["reaction_wheels"];
	assert(reactionWheelsIterator.IsArray()); 
    
    for (rapidjson::Value::ConstValueIterator itr = reactionWheelsIterator.Begin(); itr != reactionWheelsIterator.End(); ++itr) 
	{
    	const rapidjson::Value& reactionWheelPropertiesUserDefined = *itr;
    	assert(reactionWheelPropertiesUserDefined.IsObject()); // each reactionWheelPropertiesUserDefined is an object
    	for (rapidjson::Value::ConstMemberIterator itr2 = reactionWheelPropertiesUserDefined.MemberBegin(); itr2 != reactionWheelPropertiesUserDefined.MemberEnd(); ++itr2) 
		{ 
            std::string nameString = itr2->name.GetString(); 
            if ( nameString.compare( "uuid" ) == 0 ) // Extract the uuids of the reaction wheels 
            {
                actuatorUuid.push_back(  itr2->value.GetString());
            }
            else // Extract the orientations of the reaction wheel. 
            {
                Vector2 orientation( sml::convertDegreesToRadians( itr2->value[0].GetDouble() ), sml::convertDegreesToRadians( itr2->value[1].GetDouble() ) ); 

                wheelOrientation.push_back( orientation );
            }
		}
	}

    // Check if the control torque is active.
    const bool controlTorqueActiveModelFlag     = find( config, "is_control_torque_active" )->value.GetBool(); 
    std::cout << "Is control torque active?                                     " << controlTorqueActiveModelFlag << std::endl; 
    
    // Controller properties as defined by the user. 
    const Real naturalFrequency                 = find( config, "natural_frequency")->value.GetDouble(); 
    const Real dampingRatio                     = find( config, "damping_ratio")->value.GetDouble();
    const Real slewSaturationRate               = sml::convertDegreesToRadians ( find( config, "slew_saturation_rate")->value.GetDouble() ); 
    const std::string controllerType            = find( config, "controller_type")->value.GetString(); 

    // Extract file writer settings.
    const std::string metadataFilePath                = find( config, "metadata_file_path" )->value.GetString( ); 
    std::cout << "Metadata file path  " <<  metadataFilePath << std::endl;
    const std::string stateHistoryFilePath            = find( config, "state_history_file_path" )->value.GetString( ); 
    std::cout << "State history file path  " <<  stateHistoryFilePath << std::endl;  

    return monteCarloSingleSimulatorInput(  principleInertiaMin,
                                  principleInertiaMax,  
                                  initialAttitudeStateMin,
                                  initialAttitudeStateMax,
                                  referenceAttitudeState,
                                  numberOfSamples,
                                  integrator,
                                  startEpoch,
                                  endEpoch,
                                  timeStep,
                                  gravitationalParameter,
                                  radius, 
                                  semiMajorAxis,
                                  relativeTolerance,
                                  absoluteTolerance,
                                  gravityGradientAccelerationModelFlag,
                                  attitudeControlMethod,
                                  actuator, 
                                  actuatorUuid,
                                  wheelOrientation,
                                  controlTorqueActiveModelFlag,
                                  naturalFrequency, 
                                  dampingRatio, 
                                  slewSaturationRate, 
                                  controllerType,
                                  metadataFilePath,
                                  stateHistoryFilePath);
};

} // namespace dss_adcs
