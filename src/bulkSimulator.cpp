/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <boost/numeric/odeint.hpp>
// #include "boost/numeric/odeint/external/eigen/eigen.hpp"
#include <Eigen/Core>

#include <astro/astro.hpp>
#include <sml/sml.hpp>

#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/bulkSimulator.hpp"
#include "dss_adcs/dynamicalSystem.hpp"
#include "dss_adcs/getReactionWheel.hpp"
#include "dss_adcs/reactionWheelConfiguration.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/outputWriter.hpp"

namespace dss_adcs
{

void executeBulkSimulator( const rapidjson::Document& config )
{
    // Verify config parameters. Exception is thrown if any of the parameters are missing.
    const simulatorInput input = checkBulkSimulatorInput( config );

    std::cout << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << "                           Run simulator                          " << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << std::endl;

    std::cout << "The API is being called to extract the parameters ... " << std::endl;
    const std::vector< ReactionWheel > reactionWheels = getReactionWheels( input.actuator, input.actuatorUuid); 

    // Define the actuator configuration. 
    std::cout << "Defining actuator configuration ... \n" << std::endl; 
    
    const unsigned int minimumNumberOfReactionWheels    = input.numberOfReactionWheels[0]; 
    const unsigned int maximumNumberOfReactionWheels    = input.numberOfReactionWheels[1] + 1;  

    // Create instance of dynamical system.
    std::cout << "Setting up dynamical model ..." << std::endl;
    
    //Set up numerical integrator. 
    std::cout << "Executing numerical integrator ..." << std::endl;
    // Set up dynamical model.
    std::cout << "Generating angular accelerations ..." << std::endl;
    std::cout << std::endl;
    // Progress bar. 
    std::cout << "Current Progress ..." << std::endl; 
    
    std::ofstream metadatafile( input.metadataFilePath + ".csv" );
    std::map< std::pair<std::string, unsigned int>, std::vector <ReactionWheel> > reactionWheelConcepts = 
                                            getReactionWheelConcepts( input.reactionWheelConfiguration, 
                                                                      reactionWheels, 
                                                                      minimumNumberOfReactionWheels,
                                                                      maximumNumberOfReactionWheels, 
                                                                      input.wheelOrientation  ); 
    // Current value in the reaction wheel concept iterator. 
    unsigned int ii = 0; 
    
    for ( std::map< std::pair<std::string, unsigned int>, std::vector<ReactionWheel> >::iterator reactionWheelConceptIterator = reactionWheelConcepts.begin(); reactionWheelConceptIterator != reactionWheelConcepts.end(); ++reactionWheelConceptIterator )
    {
        const std::pair<std::string, unsigned int> conceptIdentifier = reactionWheelConceptIterator->first; 
        const unsigned int numberOfReactionWheels = conceptIdentifier.second; 
        // Print metadata to the file provide in metadatafile path. 
        std::ofstream metadatafile( input.metadataFilePath + std::to_string(numberOfReactionWheels) + ".csv" );
        if ( numberOfReactionWheels == 2 )
        {
            metadatafile << "concept,numberOfReactionWheel,mass,volume,rw1,rw2" << std::endl; 
        }    
        else if ( numberOfReactionWheels == 3 )
        {
            metadatafile << "concept,numberOfReactionWheel,mass,volume,rw1,rw2,rw3" << std::endl; 
        } 
        else if ( numberOfReactionWheels == 4 )
        {
            metadatafile << "concept,numberOfReactionWheel,mass,volume,rw1,rw2,rw3,rw4" << std::endl; 
        }
        else if ( numberOfReactionWheels == 5 )
        {
            metadatafile << "concept,numberOfReactionWheel,mass,volume,rw1,rw2,rw3,rw4,rw5" << std::endl; 
        }
        else if ( numberOfReactionWheels == 6 )
        {
            metadatafile << "concept,numberOfReactionWheel,mass,volume,rw1,rw2,rw3,rw4,rw5,rw6" << std::endl; 
        }
        else
        {
            std::cout << "Metadatafile incorrect for the number of reaction wheels = " << numberOfReactionWheels << std::endl; 
        }
        const std::vector< ReactionWheel > reactionWheelConcept = reactionWheelConceptIterator->second; 
        const ActuatorConfiguration actuatorConfiguration( reactionWheelConcept ); 
        // Create file stream to write state history to.`
        // std::ofstream stateHistoryFile( input.stateHistoryFilePath + ".csv");
        std::ofstream stateHistoryFile( input.stateHistoryFilePath + conceptIdentifier.first + std::to_string(numberOfReactionWheels) + ".csv");
        if ( numberOfReactionWheels == 3 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,angularMomentum1,angularMomentum2,angularMomentum3,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2reactionWheelAngularVelocity3,powerConsumption1,powerConsumption2,powerConsumption3,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl; 
        }
        else if ( numberOfReactionWheels == 2 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,angularMomentum1,angularMomentum2,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2,powerConsumption1,powerConsumption2disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        }
        else if ( numberOfReactionWheels == 4 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,motorTorque4,angularMomentum1,angularMomentum2,angularMomentum3,angularMomentum4,reactionWheelAngularVelocity1reactionWheelAngularVelocity2,reactionWheelAngularVelocity3,reactionWheelAngularVelocity4,powerConsumption1,powerConsumption2,powerConsumption3powerConsumption4,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        }
        else if ( numberOfReactionWheels == 5 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,motorTorque4,angularMomentum1,angularMomentum2,angularMomentum3,angularMomentum4,angularMomentum5,reactionWheelAngularvelocity1reactionWheelAngularvelocity2,reactionWheelAngularvelocity3,reactionWheelAngularVelocity4,reactionWheelAngularVelocity5,powerConsumption1,powerConsumption2powerConsumption3,powerConsumption4,powerConsumption5,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        }
        else if ( numberOfReactionWheels == 6 )
        {
            stateHistoryFile << "t,q1,q2,q3,q4,eulerRotationAngle,theta1,theta2,theta3,w1,w2,w3,slewRate,controlTorque1,controlTorque2,controlTorque3,motorTorque1motorTorque2,motorTorque3,motorTorque4,motorTorque5,motorTorque6,angularMomentum1,angularMomentum2,angularMomentum3,angularMomentum4,angularMomentum5angularMomentum6,reactionWheelAngularVelocity1,reactionWheelAngularVelocity2,reactionWheelAngularVelocity3,reactionWheelAngularVelocity4reactionWheelAngularVelocity5,reactionWheelAngularVelocity6,powerConsumption1,powerConsumption2,powerConsumption3,powerConsumption4,powerConsumption5powerConsumption6,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;
        }
        else
        {
            std::cout << "The state history file not set up for " << numberOfReactionWheels << " reaction wheels!" << std::endl; 
            throw; 
        }

        VectorXd currentState( ( input.initialAttitudeState.size() + reactionWheelConcept.size() ) );                 
        for ( unsigned int stateIterator = 0; stateIterator < (input.initialAttitudeState.size() + reactionWheelConcept.size() ); ++stateIterator )
        {
            if ( stateIterator < input.initialAttitudeState.size() )
            {
                currentState[stateIterator] = input.initialAttitudeState[stateIterator]; 
            }
            else if ( stateIterator >= input.initialAttitudeState.size() && stateIterator < (input.initialAttitudeState.size() +        reactionWheelConcept.size() ) )
            {
                currentState[stateIterator] = 0.0; 
            }
            else
            {
                std::cout << "Something is going wrong in the initial current state iterator! " << std::endl; 
            }
        }; 
        Vector4 referenceAttitudeState      = input.referenceAttitudeState; 

        const Vector4 initialQuaternion(input.initialAttitudeState[0], input.initialAttitudeState[1], input.initialAttitudeState[2], input.initialAttitudeState[3]); 
        for ( Real integrationStartTime = input.startEpoch; integrationStartTime < input.endEpoch; integrationStartTime++ )
        {
            const Real integrationEndTime = integrationStartTime + input.timeStep; 
            const Vector4 currentAttitude( currentState[0], currentState[1], currentState[2], currentState[3] ); 
            const Vector3 currentAttitudeRate( currentState[4], currentState[5], currentState[6] ); 
            VectorXd reactionWheelAngularMomentums( reactionWheelConcept.size() ); 
            for ( unsigned int angularMomentumIterator = 0; angularMomentumIterator < reactionWheelAngularMomentums.size(); ++angularMomentumIterator)
            { 
                reactionWheelAngularMomentums[angularMomentumIterator] = currentState[input.initialAttitudeState.size() +       angularMomentumIterator ]; 
            }
            VectorXd reactionWheelAngularVelocities = actuatorConfiguration.computeReactionWheelVelocities(reactionWheelAngularMomentums);
            
            Vector3 asymmetricBodyTorque( 0.0, 0.0, 0.0 ); 
            if ( input.asymmetricBodyTorqueModelFlag != false )
            {
                asymmetricBodyTorque    = astro::computeRotationalBodyAcceleration( input.principleInertia, currentAttitudeRate );
            }
            
            Vector3 gravityGradientTorque( 0.0, 0.0, 0.0 ); 
            // Disturbance torques. 
            if ( input.gravityGradientAccelerationModelFlag != false )
            {
                gravityGradientTorque += astro::computeGravityGradientTorque( input.gravitationalParameter, input.radius, input.principleInertia, currentAttitude ); 
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
                                                                                              input.principleInertia, 
                                                                                              initialQuaternion, 
                                                                                              integrationStartTime ); 
            Vector3 controlTorque( outputTorques.first ); 
            VectorXd reactionWheelMotorTorque( outputTorques.second );  
            if ( input.controlTorqueActiveModelFlag == false )
            {
                controlTorque = { 0.0, 0.0, 0.0 };
            }
            // Compute reaction wheel power. 
            VectorXd reactionWheelPowerConsumption   = actuatorConfiguration.computeReactionWheelPower( reactionWheelMotorTorque, 
                                                                                   reactionWheelAngularMomentums ); 
            StateHistoryWriter writer( stateHistoryFile, controlTorque, reactionWheelMotorTorque, disturbanceTorque, reactionWheelAngularVelocities,reactionWheelPowerConsumption );
            // Dynamics of the system 
            DynamicalSystem dynamics( asymmetricBodyTorque, controlTorque, disturbanceTorque, reactionWheelMotorTorque, input.principleInertia );
            
            // Convert the eigen type vector to std::vector for compatibility with boost integrators. 
            VectorXdIntegration currentStateForIntegration( currentState.data(), currentState.data() + currentState.rows() * currentState.cols() );
    
            if ( input.integrator == rk4 )
            {
                using namespace boost::numeric::odeint;
                integrate_const( runge_kutta4< VectorXdIntegration >( ),
                                 dynamics,
                                 currentStateForIntegration,
                                 integrationStartTime,
                                 integrationEndTime,
                                 input.timeStep,
                                 writer );   
            }
            else if ( input.integrator == dopri5 )
            {
                using namespace boost::numeric::odeint; 
                integrate(  dynamics, 
                            currentStateForIntegration, 
                            integrationStartTime, 
                            integrationEndTime, 
                            input.timeStep, 
                            writer ); 
            }
            else 
            {
                std::cout << "Numerical Integrator " << input.integrator << " not defined yet! " << std::endl; 
                throw; 
            }
            currentState    = VectorXd::Map( currentStateForIntegration.data(), currentStateForIntegration.size() );   
 
        }
        progressBar( reactionWheelConcepts.size(), ii ); 
        ii +=  1; 
        // std::cout << "Iterator: " << ii + 1 << std::endl; 
            
        //! Write metadata to the metadata file path. 
        if (reactionWheelConcept.size() == 3)
        {
            doPrint( metadatafile,conceptIdentifier.first + std::to_string(numberOfReactionWheels),numberOfReactionWheels, actuatorConfiguration.calculateMassBudget(), actuatorConfiguration.calculateVolumeBudget(), reactionWheelConcept[0].name,reactionWheelConcept[1].name,reactionWheelConcept[2].name );
        }
        else if ( reactionWheelConcept.size() == 4 )
        {
            doPrint( metadatafile,conceptIdentifier.first + std::to_string(numberOfReactionWheels),numberOfReactionWheels,actuatorConfiguration.calculateMassBudget(),actuatorConfiguration.calculateVolumeBudget(),reactionWheelConcept[0].name,reactionWheelConcept[1].name,reactionWheelConcept[2].name,reactionWheelConcept[3].name ); 
        }
        else if ( reactionWheelConcept.size() == 2 )
        {
            doPrint( metadatafile,conceptIdentifier.first + std::to_string(numberOfReactionWheels),numberOfReactionWheels,actuatorConfiguration.calculateMassBudget(),actuatorConfiguration.calculateVolumeBudget(),reactionWheelConcept[0].name,reactionWheelConcept[1].name ); 
        } 
        else if ( reactionWheelConcept.size() == 5 )
        {
            doPrint( metadatafile,conceptIdentifier.first + std::to_string(numberOfReactionWheels),numberOfReactionWheels,actuatorConfiguration.calculateMassBudget(),actuatorConfiguration.calculateVolumeBudget(),reactionWheelConcept[0].name,reactionWheelConcept[1].name,reactionWheelConcept[2].name,reactionWheelConcept[3].name,reactionWheelConcept[4].name ); 
        }
        else if ( reactionWheelConcept.size() == 6 )
        {
            doPrint( metadatafile,conceptIdentifier.first + std::to_string(numberOfReactionWheels),numberOfReactionWheels,actuatorConfiguration.calculateMassBudget(),actuatorConfiguration.calculateVolumeBudget(),reactionWheelConcept[0].name,reactionWheelConcept[1].name,reactionWheelConcept[2].name,reactionWheelConcept[3].name,reactionWheelConcept[4].name,reactionWheelConcept[5].name ); 
        } 
        else 
        {
            std::cout << "Cannot print the metadata the for ther number of reaction wheels!" << std::endl; 
        }
            // doPrint( metadatafile, 4, actuatorConfiguration.calculateMassBudget(), actuatorConfiguration.calculateVolumeBudget() ); 
            metadatafile << std::endl;
    }
};

//! Check input parameters for the attitude_dynamics_simulator mode. 
simulatorInput checkBulkSimulatorInput( const rapidjson::Document& config )
{
    // Extract principle inertia. 
    ConfigIterator principleInertiaDiagonalIterator     = find( config, "principle_inertia"); 
    Inertia principleInertia; 
    principleInertia[0]                                 = principleInertiaDiagonalIterator->value[0].GetDouble( ); 
    principleInertia[1]                                 = principleInertiaDiagonalIterator->value[1].GetDouble( ); 
    principleInertia[2]                                 = principleInertiaDiagonalIterator->value[2].GetDouble( );
    std::cout << "Principle inertia around X axis:                              " << principleInertia[0]
              << "[kg/m^2]" << std::endl; 
    std::cout << "Principle inertia around Y axis:                              " << principleInertia[1]
              << "[kg/m^2]" << std::endl;          
    std::cout << "Principle inertia around Z axis:                              " << principleInertia[2]
              << "[kg/m^2]" << std::endl; 
    
    // Extract attitude kinematic type. 
    const std::string attitudeRepresentationString      = find( config, "attitude_representation" )->value.GetString( );
    std::cout << "Attitude representation:                                      " << attitudeRepresentationString << std::endl; 
    
    // Extract the initial attitude states and angular velocities. 
    ConfigIterator initialAttitudeStateEulerIterator    = find( config, "initial_attitude_state");
    ConfigIterator referenceAttitudeStateIterator       = find( config, "attitude_reference_state" );
    const rapidjson::Value& intialAttitudeStateSize 	= config["initial_attitude_state"]; 

    Vector7 initialAttitudeState;
    Vector4 referenceAttitudeState; 
    Vector3 initialAttitudeStateEuler;

    if (attitudeRepresentationString.compare("euler_angles") == 0)
    {
        initialAttitudeStateEuler[0]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerIterator->value[0].GetDouble() );
        initialAttitudeStateEuler[1]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerIterator->value[1].GetDouble() );
        initialAttitudeStateEuler[2]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerIterator->value[2].GetDouble() );
	
	// Check the size of the input attitude state! 	
	if (  intialAttitudeStateSize.Size()  != 6 )
	{
		std::cout << "FATAL error! The size of the initial attitude state is more than 6 elements for the Euler kinematic representation!" << std::endl; 
		throw;	
	}

        Vector4 quaternionInitialState                 = astro::transformEulerToQuaternion( initialAttitudeStateEuler );
        initialAttitudeState[0]                        = quaternionInitialState[0];
        initialAttitudeState[1]                        = quaternionInitialState[1];
        initialAttitudeState[2]                        = quaternionInitialState[2];
        initialAttitudeState[3]                        = quaternionInitialState[3];   
        initialAttitudeState[4]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[3].GetDouble() ); 
        initialAttitudeState[5]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[4].GetDouble() ); 
        initialAttitudeState[6]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[5].GetDouble() ); 

        Vector3 referenceAttitudeStateEuler; 

        referenceAttitudeStateEuler[0]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[0].GetDouble() );
        referenceAttitudeStateEuler[1]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[1].GetDouble() );
        referenceAttitudeStateEuler[2]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[2].GetDouble() ); 

        referenceAttitudeState                         = astro::transformEulerToQuaternion( referenceAttitudeStateEuler );    
    }
    else if ( attitudeRepresentationString.compare("quaternions") == 0 )
    {
	// Check the size of the input attitude state! 	
	if (  intialAttitudeStateSize.Size()  != 7 )
	{
		std::cout << "FATAL error! The size of the initial attitude state is not equal to 7 elements for the quaternion kinematic representation!" << std::endl; 
		throw;	
	}

        initialAttitudeState[0]                        = initialAttitudeStateEulerIterator->value[0].GetDouble();
        initialAttitudeState[1]                        = initialAttitudeStateEulerIterator->value[1].GetDouble();
        initialAttitudeState[2]                        = initialAttitudeStateEulerIterator->value[2].GetDouble();
        initialAttitudeState[3]                        = initialAttitudeStateEulerIterator->value[3].GetDouble();   
        initialAttitudeState[4]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[4].GetDouble() ); 
        initialAttitudeState[5]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[5].GetDouble() ); 
        initialAttitudeState[6]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[6].GetDouble() ); 

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

    std::cout << "Initial attitude state in quaternion:                         " << initialAttitudeState[0]<< "," << initialAttitudeState[1]<< "," << initialAttitudeState[2]<< "," << initialAttitudeState[3] << std::endl;
    std::cout << "Initial attitude rates:                                       " << initialAttitudeState[4]<< "," << initialAttitudeState[5]<< "," << initialAttitudeState[6]<< " [rad/sec]" << std::endl;

    std::cout << "Reference state in quaternion:                                " << referenceAttitudeState[0]<< "," << referenceAttitudeState[1]<< "," << referenceAttitudeState[2]<< "," << referenceAttitudeState[3] << std::endl;  
    
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

    // Extract assymetric body torque flag. 
    const bool asymmetricBodyTorqueModelFlag    = find( config, "is_asymmetric_body_torque_active")->value.GetBool(); 
    std::cout << "Is asymmetric body torque model active?                       " << asymmetricBodyTorqueModelFlag << std::endl; 

    // Extract actuator model and parameters. 
    const std::string attitudeControlMethod     = find( config, "attitude_control_method")->value.GetString();
    std::cout << "Attitude control method:                                      " << attitudeControlMethod << std::endl; 
    
    // Extract the name of the actuator
    const std::string actuator                  = find( config, "actuator")->value.GetString(); 
    std::cout << "Actuator used for control:                                    " << actuator << std::endl; 

    // Extract reaction wheel configuration type. 
    const std::string reactionWheelConfiguration    = find( config, "reaction_wheel_config_type" )->value.GetString(); 
    std::cout << "The configuration of reaction wheels                          " << reactionWheelConfiguration << std::endl; 

    // Extract reaction wheel attributes.
    std::vector < Vector2 > wheelOrientation; 
    std::vector < std::string > actuatorUuid;  

    // Reaction wheel orientations defined by the user. 
    const rapidjson::Value& reactionWheelsIterator 	= config["wheel_orientation"];
	assert(reactionWheelsIterator.IsArray()); 
    for (rapidjson::Value::ConstValueIterator itr = reactionWheelsIterator.Begin(); itr !=  reactionWheelsIterator.End(); ++itr) 
	{
    	const rapidjson::Value& reactionWheelPropertiesUserDefined = *itr;
        Vector2 orientation( sml::convertDegreesToRadians( reactionWheelPropertiesUserDefined[0].GetDouble() ), sml::convertDegreesToRadians(reactionWheelPropertiesUserDefined[1].GetDouble()  ) ); 
        wheelOrientation.push_back( orientation );
    }

    // Reaction wheel Uuids defined in a separate input file
    std::string reactionWheelUuidFile   = find( config, "reaction_wheel_uuids")->value.GetString(); 

    std::ifstream uuidInputFile( reactionWheelUuidFile );
	std::stringstream jsonDocumentBuffer;
	std::string uuidInputLine;

	while ( std::getline( uuidInputFile, uuidInputLine ) )
	{
	    jsonDocumentBuffer << uuidInputLine << "\n";
	}
 
    rapidjson::Document uuidConfig;
    uuidConfig.Parse( jsonDocumentBuffer.str( ).c_str( ) );

    assert(uuidConfig.IsObject()); 

    for(rapidjson::Value::ConstMemberIterator reactionWheelNameIterator = uuidConfig.MemberBegin(); reactionWheelNameIterator != uuidConfig.MemberEnd(); ++reactionWheelNameIterator )
    {
        actuatorUuid.push_back( reactionWheelNameIterator->value.GetString() ); 
    }

    // Number of reaction wheels in a concept. 
    ConfigIterator numberOfReactionWheelIterator    = find( config, "number_of_reaction_wheels"); 
    const Vector2 numberOfReactionWheels( numberOfReactionWheelIterator->value[0].GetDouble(), numberOfReactionWheelIterator->value[1].GetDouble() ); 

    // Check if the control torque is active.
    const bool controlTorqueActiveModelFlag     = find( config, "is_control_torque_active" )->value.GetBool(); 
    std::cout << "Is control torque active?                                     " << controlTorqueActiveModelFlag << std::endl; 
    
    // Control gains for the controller.  
    const Real naturalFrequency                 = find( config, "natural_frequency")->value.GetDouble(); 
    const Real dampingRatio                     = find( config, "damping_ratio")->value.GetDouble();
    const Real slewSaturationRate               = sml::convertDegreesToRadians ( find( config, "slew_saturation_rate")->value.GetDouble() );
    const std::string controllerType            = find( config, "controller_type")->value.GetString(); 

    // Extract file writer settings.
    const std::string metadataFilePath                  = find( config, "metadata_file_path" )->value.GetString( ); 
    std::cout << "Metadata file path  " <<  metadataFilePath << std::endl;
    const std::string stateHistoryFilePath              = find( config, "state_history_file_path" )->value.GetString( ); 
    std::cout << "State history file path  " <<  stateHistoryFilePath << std::endl;  

    return simulatorInput(  principleInertia,
                            initialAttitudeState,
                            referenceAttitudeState,
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
                            asymmetricBodyTorqueModelFlag,
                            attitudeControlMethod,
                            actuator, 
                            reactionWheelConfiguration,
                            actuatorUuid,
                            wheelOrientation,
                            numberOfReactionWheels,
                            controlTorqueActiveModelFlag,
                            naturalFrequency, 
                            dampingRatio, 
                            slewSaturationRate, 
                            controllerType,
                            metadataFilePath,
                            stateHistoryFilePath    );
};

} // namespace dss_adcs
