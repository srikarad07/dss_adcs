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
// #include <integrate/integrate.hpp>

#include "dss_adcs/dynamicalSystem.hpp"
#include "dss_adcs/simulator.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/outputWriter.hpp"
// #include "dss_adcs/rungeKutta.hpp"

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

    // Create instance of dynamical system.
    std::cout << "Setting up dynamical model ..." << std::endl;
    
    /*  testInt for the gravity gradient model 
    *   assumptions: raidius is an assumption -> ideally radius should be derived from a ephermeris. 
    *   assumption: random direction cosine matrix is defined -> todo make a function for 
    *   direction cosines and conversion from euler angles and quaternions. 
    *   assumtion: random gravitational parameter defined for now -> ideally should be 
    *   taken from a reliable source. 
    */
    Real radius = 500.0; 
    Real gravitationalParameter = 1000000.0;
    Matrix33 directionCosineMatrix(3,3); 

    // boost::numeric::ublas::matrix< Real > directionCosineMatrix(3,3);
    directionCosineMatrix(0,0) = 1.0;
    directionCosineMatrix(0,1) = 0.0;
    directionCosineMatrix(0,2) = 0.0;
    directionCosineMatrix(1,0) = 0.0; 
    directionCosineMatrix(1,1) = 1.0;
    directionCosineMatrix(1,2) = 0.0;
    directionCosineMatrix(2,0) = 0.0;
    directionCosineMatrix(2,1) = 0.0;
    directionCosineMatrix(2,2) = 1.0;

    DynamicalSystem dynamics( input.principleInertia, gravitationalParameter, radius, directionCosineMatrix );
    std::cout << "Dynamical model set up successfully!" << std::endl;
    std::cout << std::endl;

    // Create file stream to write state history to.
    std::ofstream stateHistoryFile( input.stateHistoryFilePath );
    stateHistoryFile << "t, roll, pitch, yaw, roll_rate, pitch_rate, yaw_rate" << std::endl;
    StateHistoryWriter writer( stateHistoryFile );

    //Set up numerical integrator. 
    std::cout << "Executing numerical integrator ..." << std::endl;
    State currentState = input.initialAttitudeState;
    if ( input.integrator == rk4 )
    {
        using namespace boost::numeric::odeint;
        integrate_const( runge_kutta4< State,double,State,double,vector_space_algebra >( ),
                         dynamics,
                         currentState,
                         input.startEpoch,
                         input.endEpoch,
                         input.timeStep,
                         writer );
        // double stepSize = steps.size();
        // std::cout << "Size of the integration: " << stepSize << std::endl; 
        // integrate::performRungeKutta4Integration( dynamics, input );
        // std::cout << "Numerical Integrator" << input.integrator << "run successfully!" << std::endl;
    }
    else if ( input.integrator == testInt )
    { 
        // std::cout << "Previous Current state: " << currentState[0] << std::endl; 

        // using namespace boost::numeric::odeint;
        // size_t steps = integrate(   dynamics, 
        //                             currentState, 
        //                             input.startEpoch,
        //                             input.endEpoch,
        //                             input.timeStep ); 
        // std::cout << "After integration Curent state: " << currentState[0] << std::endl;     
    }
    else
    {
        std::cout << "Numerical integrator not defined" << std::endl;
        throw;
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
    State initialAttitudeState;
    initialAttitudeState[0]                             = initialAttitudeStateIterator->value[0].GetDouble();
    initialAttitudeState[1]                             = initialAttitudeStateIterator->value[1].GetDouble();
    initialAttitudeState[2]                             = initialAttitudeStateIterator->value[2].GetDouble();
    initialAttitudeState[3]                             = initialAttitudeStateIterator->value[3].GetDouble();
    initialAttitudeState[4]                             = initialAttitudeStateIterator->value[4].GetDouble(); 
    initialAttitudeState[5]                             = initialAttitudeStateIterator->value[5].GetDouble(); 
    std::cout << "Roll angle:                       "   << initialAttitudeState[0]
              << "[deg]"           << std::endl; 
    std::cout << "Pitch angle:                      "   << initialAttitudeState[1]
              << "[deg]"           << std::endl;
    std::cout << "Yaw angle                         "   << initialAttitudeState[2]
              << "[deg]"           << std::endl;
    std::cout << "Roll rate:                        "   << initialAttitudeState[3]
              << "[deg/sec]"       << std::endl; 
    std::cout << "Pitch rate:                       "   << initialAttitudeState[4]
              << "[deg/sec]"       << std::endl;
    std::cout << "[Yaw rate]                        "   << initialAttitudeState[5]
              << "[deg/sec]"       << std::endl;

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
    
    // Extract integrator tolerances. 
    const Real relativeTolerance                      = find( config, "relative_tolerance")->value.GetDouble(); 
    std::cout << "Relative Tolerance: " << relativeTolerance
              << "[-]" << std::endl; 
    const Real absoluteTolerance                      = find( config, "absolute_tolerance")->value.GetDouble(); 
    std::cout << "Absolute Tolerance: " << absoluteTolerance
              << "[-]" << std::endl;

    // Extract file writer settings.
    const std::string metadataFilePath                = find( config, "metadata_file_path" )->value.GetString( ); 
    std::cout << "Metadata file path  " <<  metadataFilePath << std::endl;
    const std::string stateHistoryFilePath            = find( config, "state_history_file_path" )->value.GetString( ); 
    std::cout << "State history file path  " <<  stateHistoryFilePath << std::endl;  

    return simulatorInput(  principleInertia,
                            initialAttitudeState,
                            integrator,
                            startEpoch,
                            endEpoch,
                            timeStep,
                            relativeTolerance,
                            absoluteTolerance,
                            metadataFilePath,
                            stateHistoryFilePath);
};

} // namespace dss_adcs
