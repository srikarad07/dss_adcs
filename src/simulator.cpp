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

#include "dss_adcs/apiCall.hpp"
#include "dss_adcs/dynamicalSystem.hpp"
// #include "dss_adcs/dynamicalSystemSetup.hpp"
#include "dss_adcs/productSearch.hpp"
#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/simulator.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/outputWriter.hpp"

namespace dss_adcs
{

void executeSimulator( const rapidjson::Document& config )
{
    // Verify config parameters. Exception is thrown if any of the parameters are missing.
    const simulatorInput input = checkSimulatorInput( config );

    std::cout << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << "                           Run simulator                          " << std::endl;
    std::cout << "******************************************************************" << std::endl;
    std::cout << std::endl;

    // <<<<<<<<<<<<<<<<<<<<<<< Hardcoded values >>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
    std::cout << "The API is being called to extract the parameters ... " << std::endl;
    callTheApi( input.actuator, input.actuatorUuid ); 
    
    // // <<<<<<<<<<<<<<<<<< Test script >>>>>>>>>>>>>>>>>>>> //
    // // std::cout << "<<<<<<<<<<< Start of the test script >>>>>>>>>>>>>>>>>>>>>" << std::endl; 
    // // dss_adcs::ProductSearch product; 
    // // product(); 
    // // std::cout << "<<<<<<<<<<< End of test script >>>>>>>>>>>>>>>>>>>>" << std::endl; 
    // // <<<<<<<<<<<<<<<<<<<<<< End test script >>>>>>>>>>>>>>>>>> //
    
    const ReactionWheel rw1( 0.0, 0.0, 0.0, 0.0, 0.1 ), rw2( 0.0, 0.0, 0.0, 0.0, 0.1 ), rw3( 0.0, 0.0, 0.0, 0.0, 0.1 ); 

    // Define the actuator configuration. 
    std::cout << "The actuator configuration is being defined: \n" << std::endl; 
    const ActuatorConfiguration actuatorConfiguration( rw1, rw2, rw3 ); 

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
    stateHistoryFile << "t,q1,q2,q3,q4,w1,w2,w3,tow1,tow2,tow3" << std::endl;

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
        
        // Get the accelerations for the model. 
        // DynamicalSystemSetup dynamicsSetup( currentState, refrenceState, input.timeStep, input.principleInertia, integrationStartTime, actuatorConfiguration ); 

        Real integrationEndTime = integrationStartTime + input.timeStep; 

        Vector4 currentAttitude( currentState[0], currentState[1], currentState[2], currentState[3] ); 
        Vector3 currentAttitudeRate( currentState[4], currentState[5], currentState[6] ); 

        Vector3 torque = astro::computeRotationalBodyAcceleration( input.principleInertia, currentAttitudeRate );

        // Disturbance torques. 
        // if ( gravityGradientAccelerationModelFlag == true )
        // {
        //        Matrix33 directionCosineMatrix( astro::computeEulerAngleToDcmConversionMatrix(rotationSequence, currentAttitude) );

        //        acceleration += astro::computeGravityGradientTorque( gravitationalParameter, radius, principleInertia, directionCosineMatrix ); 
 
        // }

        // Control torque on the dynamics: 
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<< ASSUMPTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
        Vector3 quaternionControlGainMatrix( 10.0, 10.0, 10.0 );
        Vector3 angularVelocityControlGainMatrix( 10.0, 10.0, 10.0 ); 
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<  End of assumptions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
        // TO DO: check the controller applicability. Nonlinearity of the equations as well need to be checked //
        
        Vector3 controlTorque = dss_adcs::computeRealTorqueValue(   currentAttitude, 
                                                                    referenceAttitudeState,
                                                                    currentAttitudeRate, 
                                                                    quaternionControlGainMatrix, 
                                                                    angularVelocityControlGainMatrix, 
                                                                    actuatorConfiguration );

        torque  += controlTorque; 

        // Angular acceleration on the spacecraft. // 
        Vector3 acceleration; 
        acceleration[0]     = torque[0] / input.principleInertia[0]; 
        acceleration[1]     = torque[1] / input.principleInertia[1]; 
        acceleration[2]     = torque[2] / input.principleInertia[2];   
        
        StateHistoryWriter writer( stateHistoryFile, controlTorque );
            
        // Dynamics of the system 
        DynamicalSystem dynamics( input.principleInertia, input.gravitationalParameter, input.radius, input.semiMajorAxis,  input.gravityGradientAcclerationModelFlag, actuatorConfiguration, acceleration );

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

};

//! Check input parameters for the attitude_dynamics_simulator mode. 
simulatorInput checkSimulatorInput( const rapidjson::Document& config )
{
    // Extract principle inertia. 
    ConfigIterator principleInertiaDiagonalIterator     = find( config, "principle_inertia"); 
    Inertia principleInertia; 
    principleInertia[0]                                 = principleInertiaDiagonalIterator->value[0].GetDouble( ); 
    principleInertia[1]                                 = principleInertiaDiagonalIterator->value[1].GetDouble( ); 
    principleInertia[2]                                 = principleInertiaDiagonalIterator->value[2].GetDouble( );
    std::cout << "Principle inertia around X axis:    " << principleInertia[0]
              << "[kg/m^2]" << std::endl; 
    std::cout << "Principle inertia around Y axis:    " << principleInertia[1]
              << "[kg/m^2]" << std::endl;          
    std::cout << "Principle inertia around Z axis:    " << principleInertia[2]
              << "[kg/m^2]" << std::endl;

    // Extract the initial attitude states and angular velocities. 
    ConfigIterator initialAttitudeStateIterator         = find( config, "initial_attitude_state"); 
    
    // Extract attitude kinematic type. 
    const std::string attitudeRepresentationString      = find( config, "attitude_representation" )->value.GetString( );
    std::cout << "Attitude representation: " << attitudeRepresentationString << std::endl; 
    
    // Extract the initial state of the quaternion. 
    Vector7 initialAttitudeState;
    initialAttitudeState[0]                             = initialAttitudeStateIterator->value[0].GetDouble();
    initialAttitudeState[1]                             = initialAttitudeStateIterator->value[1].GetDouble();
    initialAttitudeState[2]                             = initialAttitudeStateIterator->value[2].GetDouble();
    initialAttitudeState[3]                             = initialAttitudeStateIterator->value[3].GetDouble();
    initialAttitudeState[4]                             = sml::convertDegreesToRadians(initialAttitudeStateIterator->value[4].GetDouble()); 
    initialAttitudeState[5]                             = sml::convertDegreesToRadians(initialAttitudeStateIterator->value[5].GetDouble()); 
    initialAttitudeState[6]                             = sml::convertDegreesToRadians(initialAttitudeStateIterator->value[6].GetDouble());    

    // Extract the reference state for the quaternion. 
    ConfigIterator referenceAttitudeStateIterator       = find( config, "quaternion_reference_state" );
    Vector4 referenceAttitudeState;
    referenceAttitudeState[0]                           = referenceAttitudeStateIterator->value[0].GetDouble();
    referenceAttitudeState[1]                           = referenceAttitudeStateIterator->value[1].GetDouble();
    referenceAttitudeState[2]                           = referenceAttitudeStateIterator->value[2].GetDouble();    
    referenceAttitudeState[3]                           = referenceAttitudeStateIterator->value[3].GetDouble();      
      
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
    std::cout << "Integrator                         " << integratorString << std::endl;

    // Extract integrator time setttings. 
    const Real startEpoch                              = find( config, "start_epoch")->value.GetDouble( );
    std::cout << "Intergration start epoch:         " << startEpoch 
              << "[sec]"        << std::endl; 
    const Real endEpoch                                = find( config, "end_epoch")->value.GetDouble( );
    std::cout << "Integration end epoch:            " << endEpoch 
              << "[sec]" << std::endl;
    const Real timeStep                                = find( config, "time_step" )->value.GetDouble( );
    std::cout << "Timestep of the integration is:   " << timeStep 
              << "[sec]" << std::endl; 
    
    // Extract gravitational parameter of the central body.  
    const Real gravitationalParameter                 = find( config, "gravitational_parameter")->value.GetDouble(); 
    std::cout << "Gravitational Parameter: " << gravitationalParameter
              << "[km^3 s^-2]" << std::endl; 

    // Extract the radial distance of the central body. 
    const Real radius                                 = find( config, "radius")->value.GetDouble();
    std::cout << "Radial vector: "      << radius
              << "[km]" << std::endl; 

    // Exract the semi major axis of the orbit. 
    const Real semiMajorAxis                          = find( config, "semi_major_axis")->value.GetDouble(); 
    std::cout << "Semi major axis: "   << semiMajorAxis
              << "[km]" << std::endl; 

    // Extract integrator tolerances.  
    const Real relativeTolerance                      = find( config, "relative_tolerance")->value.GetDouble(); 
    std::cout << "Relative Tolerance: " << relativeTolerance
              << "[-]" << std::endl; 
    const Real absoluteTolerance                      = find( config, "absolute_tolerance")->value.GetDouble(); 
    std::cout << "Absolute Tolerance: " << absoluteTolerance
              << "[-]" << std::endl;

    // Extract gravity gradient model .
    const bool gravityGradientAcclerationModelFlag = find( config, "is_gravity_gradient_active" )->value.GetBool( );
    std::cout << "Is Gravity Gradient disturbance model active?                " << gravityGradientAcclerationModelFlag << std::endl;

    // Extract actuator model and parameters. 
    const std::string conceptConfiguration     = find( config, "concept_configuration")->value.GetString();
    const std::string actuator                 = find( config, "actuator")->value.GetString(); 
    const std::string conceptType              = find( config, "type" )->value.GetString(); 
    const std::string actuatorUuid             = find( config, "uuid" )->value.GetString(); 
    // const std::string attributeUuid         = conceptIterator["uuid"]; 

    // Extract file writer settings.
    const std::string metadataFilePath                = find( config, "metadata_file_path" )->value.GetString( ); 
    std::cout << "Metadata file path  " <<  metadataFilePath << std::endl;
    const std::string stateHistoryFilePath            = find( config, "state_history_file_path" )->value.GetString( ); 
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
                            gravityGradientAcclerationModelFlag,
                            conceptConfiguration,
                            actuator, 
                            conceptType,
                            actuatorUuid,
                            metadataFilePath,
                            stateHistoryFilePath);
};

} // namespace dss_adcs
