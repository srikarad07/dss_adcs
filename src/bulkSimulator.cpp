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
    std::vector< ReactionWheel > reactionWheels; 
    reactionWheels  = getReactionWheels( input.actuator, input.actuatorUuid, input.wheelOrientation ); 

    // Define the actuator configuration. 
    std::cout << "Defining actuator configuration ... \n" << std::endl; 
    
    const int numberOfReactionWheels = 4;

    std::map< std::string, std::vector <ReactionWheel> > reactionWheelConcepts = getReactionWheelConcepts( input.reactionWheelConfiguration, 
                                                                                                           reactionWheels, 
                                                                                                           numberOfReactionWheels ); 

    for ( std::map< std::string, std::vector<ReactionWheel> >::iterator reactionWheelConceptIterator = reactionWheelConcepts.begin(); reactionWheelConceptIterator != reactionWheelConcepts.end(); ++reactionWheelConceptIterator )
    {
        std::vector< ReactionWheel > reactionWheelConcept = reactionWheelConceptIterator->second; 

        std::cout << "Reaction wheel name: " << reactionWheelConcept[0].name << std::endl; 
        std::cout << "Reaction wheel name: " << reactionWheelConcept[1].name << std::endl; 
        std::cout << "Reaction wheel name: " << reactionWheelConcept[2].name << std::endl; 
        std::cout << "Reaction wheel name: " << reactionWheelConcept[3].name << std::endl; 

        // TO DO: Move the wheel orientation as a property of the reaction wheel //
        ActuatorConfiguration actuatorConfiguration( reactionWheelConcept ); 

        // Create instance of dynamical system.
        std::cout << "Setting up dynamical model ..." << std::endl;

        /*  testInt for the gravity gradient model 
        *   assumptions: radius is an assumption -> ideally radius should be derived from a ephemeris. 
        *   direction cosines and conversion from euler angles and quaternions. 
        *   assumption: random gravitational parameter defined for now -> ideally should be 
        *   taken from a reliable source. 
        */

        // Create file stream to write state history to.
        std::ofstream stateHistoryFile( input.stateHistoryFilePath );
        stateHistoryFile << "t,q1,q2,q3,q4,theta1,theta2,theta3,w1,w2,w3,controlTorque1,controlTorque2,controlTorque3,motorTorque1,motorTorque2,motorTorque3,disturbanceTorque1,disturbanceTorque2,disturbanceTorque3" << std::endl;

        //Set up numerical integrator. 
        std::cout << "Executing numerical integrator ..." << std::endl;
        State   currentState                = input.initialAttitudeState;
        Vector4 referenceAttitudeState      = input.referenceAttitudeState; 

        // Set up dynamical model.
        std::cout << "Dynamical model setting up ..." << std::endl;

        // Set up dynamical model.
        std::cout << "Generating angular accelerations ..." << std::endl;
        std::cout << std::endl;

        for ( Real integrationStartTime = input.startEpoch; integrationStartTime < input.endEpoch; integrationStartTime++ )
        {
            Real integrationEndTime = integrationStartTime + input.timeStep; 

            Vector4 currentAttitude( currentState[0], currentState[1], currentState[2], currentState[3] ); 
            Vector3 currentAttitudeRate( currentState[4], currentState[5], currentState[6] ); 

            const Vector3 asymmetricBodyTorque    = astro::computeRotationalBodyAcceleration( input.principleInertia, currentAttitudeRate );

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

            Vector3 controlTorque( 0.0, 0.0, 0.0 ); 
            if ( input.controlTorqueActiveModelFlag != 0 )
            {
                controlTorque = dss_adcs::computeRealTorqueValue( currentAttitude, 
                                                                  referenceAttitudeState,
                                                                  currentAttitudeRate, 
                                                                  input.quaternionControlGain, 
                                                                  input.angularVelocityControlGainVector, 
                                                                  actuatorConfiguration );
            }

            StateHistoryWriter writer( stateHistoryFile, controlTorque, actuatorConfiguration.reactionWheelMotorTorque, disturbanceTorque );

            // Dynamics of the system 
            DynamicalSystem dynamics( asymmetricBodyTorque, controlTorque, disturbanceTorque, input.principleInertia );

            if ( input.integrator == rk4 )
            {
                using namespace boost::numeric::odeint;
                integrate_const( runge_kutta4< Vector7, double, Vector7, double, vector_space_algebra >( ),
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
        }
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
    ConfigIterator initialAttitudeStateEulerIterator         = find( config, "initial_attitude_state");

    // Extract the initial state of the quaternion. 
    Vector7 initialAttitudeState;

    Vector3 initialAttitudeStateEuler; 

    initialAttitudeStateEuler[0]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerIterator->value[0].GetDouble() );
    initialAttitudeStateEuler[1]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerIterator->value[1].GetDouble() );
    initialAttitudeStateEuler[2]                   = sml::convertDegreesToRadians( initialAttitudeStateEulerIterator->value[2].GetDouble() );
    
    Vector4 quaternionInitialState                 = astro::transformEulerToQuaternion( initialAttitudeStateEuler );
    initialAttitudeState[0]                        = quaternionInitialState[0];
    initialAttitudeState[1]                        = quaternionInitialState[1];
    initialAttitudeState[2]                        = quaternionInitialState[2];
    initialAttitudeState[3]                        = quaternionInitialState[3];   
    initialAttitudeState[4]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[3].GetDouble() ); 
    initialAttitudeState[5]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[4].GetDouble() ); 
    initialAttitudeState[6]                        = sml::convertDegreesToRadians(initialAttitudeStateEulerIterator->value[5].GetDouble() ); 

    std::cout << "Initial state in quaternion:                                  " << quaternionInitialState[0] << "," <<  quaternionInitialState[1] << "," << quaternionInitialState[2] << "," << quaternionInitialState[3] << std::endl;    

    // Extract the reference state for the quaternion. 
    ConfigIterator referenceAttitudeStateIterator       = find( config, "quaternion_reference_state" );
    Vector3 referenceAttitudeStateEuler; 

    referenceAttitudeStateEuler[0]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[0].GetDouble() );
    referenceAttitudeStateEuler[1]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[1].GetDouble() );
    referenceAttitudeStateEuler[2]                 = sml::convertDegreesToRadians( referenceAttitudeStateIterator->value[2].GetDouble() );  
    
    const Vector4 referenceAttitudeState           = astro::transformEulerToQuaternion( referenceAttitudeStateEuler );      

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

    const rapidjson::Value& reactionWheelsIterator 	= config["wheel_orientation"];
	assert(reactionWheelsIterator.IsArray()); 
    
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
        
    for (rapidjson::Value::ConstValueIterator itr = reactionWheelsIterator.Begin(); itr !=  reactionWheelsIterator.End(); ++itr) 
	{
    	const rapidjson::Value& reactionWheelPropertiesUserDefined = *itr;
        Vector2 orientation( sml::convertDegreesToRadians( reactionWheelPropertiesUserDefined[0].GetDouble() ), sml::convertDegreesToRadians(reactionWheelPropertiesUserDefined[1].GetDouble()  ) ); 
        wheelOrientation.push_back( orientation );
    }
       
    
    // Check if the control torque is active.
    const bool controlTorqueActiveModelFlag     = find( config, "is_control_torque_active" )->value.GetBool(); 
    std::cout << "Is control torque active?                                     " << controlTorqueActiveModelFlag << std::endl; 
    
    // Control gains for the controller. 
    const Real quaternionControlGain                    = find( config, "attitude_control_gain")->value.GetDouble(); 
    ConfigIterator angularVelocityControlGainIterator   = find( config, "angular_velocity_control_gains");
    Vector3 angularVelocityControlGainVector; 
    angularVelocityControlGainVector[0]                 =  angularVelocityControlGainIterator->value[0].GetDouble(); 
    angularVelocityControlGainVector[1]                 =  angularVelocityControlGainIterator->value[1].GetDouble();
    angularVelocityControlGainVector[2]                 =  angularVelocityControlGainIterator->value[2].GetDouble();

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
                            attitudeControlMethod,
                            actuator, 
                            reactionWheelConfiguration,
                            actuatorUuid,
                            wheelOrientation,
                            controlTorqueActiveModelFlag,
                            quaternionControlGain,
                            angularVelocityControlGainVector,
                            metadataFilePath,
                            stateHistoryFilePath    );
};

} // namespace dss_adcs
