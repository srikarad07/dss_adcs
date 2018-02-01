// /*
//  * Copyright (c) 2017, K. Kumar, Delft University of Technology (me@kartikkumar.com)
//  * Distributed under the MIT License.
//  * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
//  */

// #include <fstream>
// #include <iostream>
// #include <stdexcept>

// #include <boost/numeric/odeint.hpp>

// #include <astro/astro.hpp>

// #include "dustsim/dynamicalSystem.hpp"
// #include "dustsim/outputWriter.hpp"
// #include "dustsim/singleParticleSimulator.hpp"
// #include "dustsim/tools.hpp"

// namespace dustsim
// {

// //! Execute single particle simulator.
// void executeSingleParticleSimulator( const rapidjson::Document& config )
// {
//     // Verify config parameters. Exception is thrown if any of the parameters are missing.
//     const SingleParticleSimulatorInput input = checkSingleParticleSimulatorInput( config );

//     std::cout << std::endl;
//     std::cout << "******************************************************************" << std::endl;
//     std::cout << "                           Run simulator                          " << std::endl;
//     std::cout << "******************************************************************" << std::endl;
//     std::cout << std::endl;

//     // Compute initial state in Cartesian elements.
//     State initialState = astro::convertKeplerianToCartesianElements(
//         input.initialStateKeplerianElements, input.gravitationalParameter );
//     std::cout << "Cartesian initial state            (";
//     for ( unsigned int i = 0; i < initialState.size( ) - 1; i++ )
//     {
//         std::cout << initialState[ i ] << ", ";
//     }
//     std::cout << initialState[ initialState.size( ) - 1 ] << ")" << std::endl;
//     std::cout << std::endl;

//     // Set current state to initial state.
//     State currentState = initialState;

//     // // Set current epoch to start epoch.
//     // Real currentEpoch = input.startEpoch;

//     // Create instance of dynamical system.
//     std::cout << "Setting up dynamical model ..." << std::endl;
//     DynamicalSystem dynamics( input.gravitationalParameter,
//                               input.isJ2AccelerationModelActive,
//                               input.j2Coefficient,
//                               input.equatorialRadius,
//                               input.isRadiationPressureAccelerationModelActive,
//                               input.particleRadius,
//                               input.particleBulkDensity,
//                               input.radiationPressureCoefficient );
//     std::cout << "Dynamical model set up successfully!" << std::endl;
//     std::cout << std::endl;

//     // Write metadata to file.
//     std::ofstream metadataFile( input.metadataFilePath );
//     print( metadataFile, "gravitational_parameter", input.gravitationalParameter, "km^{3} s^{-2}" );
//     metadataFile << std::endl;
//     print( metadataFile, "j2_coefficient", input.j2Coefficient, "-" );
//     metadataFile << std::endl;
//     print( metadataFile, "equatorial_radius", input.equatorialRadius, "km" );
//     metadataFile << std::endl;
//     std::ostringstream initialKeplerStateString;
//     initialKeplerStateString << input.initialStateKeplerianElements[ astro::semiMajorAxisIndex ];
//     initialKeplerStateString << "; ";
//     initialKeplerStateString << input.initialStateKeplerianElements[ astro::eccentricityIndex ];
//     initialKeplerStateString << "; ";
//     initialKeplerStateString
//         << sml::convertRadiansToDegrees(
//             input.initialStateKeplerianElements[ astro::inclinationIndex ] );
//     initialKeplerStateString << "; ";
//     initialKeplerStateString
//         << sml::convertRadiansToDegrees(
//             input.initialStateKeplerianElements[ astro::argumentOfPeriapsisIndex ] );
//     initialKeplerStateString << "; ";
//     initialKeplerStateString
//         << sml::convertRadiansToDegrees(
//             input.initialStateKeplerianElements[ astro::longitudeOfAscendingNodeIndex ] );
//     initialKeplerStateString << "; ";
//     initialKeplerStateString
//         << sml::convertRadiansToDegrees(
//             input.initialStateKeplerianElements[ astro::trueAnomalyIndex ] );
//     print( metadataFile, "initial_state_kepler", initialKeplerStateString.str( ), "km | deg" );
//     metadataFile << std::endl;
//     print( metadataFile, "start_epoch", input.startEpoch, "s" );
//     metadataFile << std::endl;
//     print( metadataFile, "end_epoch", input.endEpoch, "s" );
//     metadataFile << std::endl;
//     print( metadataFile, "time_step", input.timeStep, "s" );
//     metadataFile << std::endl;


//     // Create file stream to write state history to.
//     std::ofstream stateHistoryFile( input.stateHistoryFilePath );
//     stateHistoryFile << "t,x,y,z,xdot,ydot,zdot,a,e,i,aop,raan,ta" << std::endl;
//     StateHistoryWriter writer( stateHistoryFile,
//                                input.gravitationalParameter );

//     // Set up numerical integrator.
//     std::cout << "Exeucting numerical integration ..." << std::endl;
//     if ( input.integrator == rk4 )
//     {
//         using namespace boost::numeric::odeint;
//         integrate_const( runge_kutta4< State >( ),
//                          dynamics,
//                          currentState,
//                          input.startEpoch,
//                          input.endEpoch,
//                          input.timeStep,
//                          writer );
//     }
//     else if ( input.integrator == dopri5 )
//     {
//         using namespace boost::numeric::odeint;
//         integrate_const( make_dense_output( input.relativeTolerance,
//                                             input.absoluteTolerance,
//                                             runge_kutta_dopri5< State >( ) ),
//                          dynamics,
//                          currentState,
//                          input.startEpoch,
//                          input.endEpoch,
//                          input.timeStep,
//                          writer );
//     }
//     else if ( input.integrator == bs )
//     {
//         using namespace boost::numeric::odeint;
//         bulirsch_stoer_dense_out< State > stepper( input.absoluteTolerance,
//                                                    input.relativeTolerance );
//         integrate_const( stepper,
//                          dynamics,
//                          currentState,
//                          input.startEpoch,
//                          input.endEpoch,
//                          input.timeStep,
//                          writer );
//     }
//     std::cout << "Numerical integrator executed successfully!" << std::endl;
// }

// //! Check input parameters for single_particle_simulator application mode.
// SingleParticleSimulatorInput checkSingleParticleSimulatorInput( const rapidjson::Document& config )
// {
//     // Extract central gravity model parameters.
//     const Real gravitationalParameter
//         = find( config, "gravitational_parameter" )->value.GetDouble( );
//     std::cout << "Gravitational parameter            " << gravitationalParameter
//               << " [km^3 s^-2]" << std::endl;

//     // Extract J2 gravity model parameters.
//     const bool j2AcclerationModelFlag = find( config, "is_j2_active" )->value.GetBool( );
//     std::cout << "Is J2 model active?                " << j2AcclerationModelFlag << std::endl;

//     const Real j2Coefficient = find( config, "j2_coefficient" )->value.GetDouble( );
//     std::cout << "J2 coefficient                     " << j2Coefficient << " [-]" << std::endl;

//     const Real equatorialRadius
//         = find( config, "equatorial_radius" )->value.GetDouble( );
//     std::cout << "Equatorial radius                  " << equatorialRadius << " [km]" << std::endl;

//     // Extract radiation pressure model parameters.
//     const bool radiationPressureFlag
//         = find( config, "is_radiation_pressure_active" )->value.GetBool( );
//     std::cout << "Is SRP model active?               "
//               << ( radiationPressureFlag ? "true" : "false" ) << std::endl;

//     const Real particleRadius = find( config, "particle_radius" )->value.GetDouble( );
//     std::cout << "Particle radius                    "
//               << particleRadius << " [micron]" << std::endl;

//     const Real particleBulkDensity = find( config, "particle_bulk_density" )->value.GetDouble( );
//     std::cout << "Particle bulk density              "
//               << particleBulkDensity << " [kg m^-3]" << std::endl;

//     const Real radiationPressureCoefficient
//         = find( config, "radiation_pressure_coefficient" )->value.GetDouble( );
//     std::cout << "Radiation pressure coefficient     "
//               << radiationPressureCoefficient << " [-]" << std::endl;

//     // Extract initial state of dust particle in Keplerian elements.
//     ConfigIterator initialStateKeplerianElementsIterator = find( config, "initial_state_kepler" );
//     State initialStateKeplerianElements;
//     initialStateKeplerianElements[ astro::semiMajorAxisIndex ]
//      = initialStateKeplerianElementsIterator->value[ astro::semiMajorAxisIndex ].GetDouble( );
//     initialStateKeplerianElements[ astro::eccentricityIndex ]
//      = initialStateKeplerianElementsIterator->value[ astro::eccentricityIndex ].GetDouble( );
//     initialStateKeplerianElements[ astro::inclinationIndex ]
//      = sml::convertDegreesToRadians(
//          initialStateKeplerianElementsIterator->value[ astro::inclinationIndex ].GetDouble( ) );
//     initialStateKeplerianElements[ astro::argumentOfPeriapsisIndex ]
//      = sml::convertDegreesToRadians(
//          initialStateKeplerianElementsIterator->value[
//              astro::argumentOfPeriapsisIndex ].GetDouble( ) );
//     initialStateKeplerianElements[ astro::longitudeOfAscendingNodeIndex ]
//      = sml::convertDegreesToRadians(
//          initialStateKeplerianElementsIterator->value[
//              astro::longitudeOfAscendingNodeIndex ].GetDouble( ) );
//     initialStateKeplerianElements[ astro::trueAnomalyIndex ]
//      = sml::convertDegreesToRadians(
//          initialStateKeplerianElementsIterator->value[ astro::trueAnomalyIndex ].GetDouble( ) );
//     std::cout << "Initial state (Kepler)             ("
//            << initialStateKeplerianElements[ astro::semiMajorAxisIndex ] << ", "
//            << initialStateKeplerianElements[ astro::eccentricityIndex ] << ", "
//            << initialStateKeplerianElements[ astro::inclinationIndex ] << ", "
//            << initialStateKeplerianElements[ astro::argumentOfPeriapsisIndex ] << ", "
//            << initialStateKeplerianElements[ astro::longitudeOfAscendingNodeIndex ] << ", "
//            << initialStateKeplerianElements[ astro::trueAnomalyIndex ] << ") "
//            << "[km, -, rad, rad, rad, rad]" << std::endl;


//     // Extract selected numerical integrator.
//     const std::string integratorString = find( config, "integrator" )->value.GetString( );
//     Integrator integrator = rk4;
//     if ( integratorString.compare( "rk4" ) != 0 )
//     {
//         if ( integratorString.compare( "rkf78" ) == 0 )
//         {
//             integrator = rkf78;
//         }
//         else if ( integratorString.compare( "dopri5" ) == 0 )
//         {
//             integrator = dopri5;
//         }
//         else if ( integratorString.compare( "bs" ) == 0 )
//         {
//             integrator = bs;
//         }
//         else
//         {
//             std::cout << std::endl;
//             std::cerr << "Selected numerical integrator \""
//                       << integratorString
//                       << "\" is incorrect!" << std::endl;
//             throw;
//         }
//     }
//     std::cout << "Integrator                         " << integratorString << std::endl;

//     // Extract integrator time settings.
//     const Real startEpoch           = find( config, "start_epoch" )->value.GetDouble( );
//     std::cout << "Start epoch                        " << startEpoch << " [s]" << std::endl;
//     const Real endEpoch             = find( config, "end_epoch" )->value.GetDouble( );
//     std::cout << "End epoch                          " << endEpoch << " [s]" << std::endl;
//     const Real timeStep             = find( config, "time_step" )->value.GetDouble( );
//     std::cout << "Time step                          " << timeStep << " [s]" << std::endl;

//     // Extract integrator tolerances.
//     const Real relativeTolerance    = find( config, "relative_tolerance" )->value.GetDouble( );
//     std::cout << "Relative tolerance                 " << relativeTolerance << " [-]" << std::endl;
//     const Real absoluteTolerance    = find( config, "absolute_tolerance" )->value.GetDouble( );
//     std::cout << "Absolute tolerance                 " << absoluteTolerance << " [-]" << std::endl;

//     // Extract file writer settings.
//     const std::string metadataFilePath
//         = find( config, "metadata_file_path" )->value.GetString( );
//     std::cout << "Metadata file path                 " << metadataFilePath << std::endl;
//     const std::string stateHistoryFilePath
//         = find( config, "state_history_file_path" )->value.GetString( );
//     std::cout << "State history file path            " << stateHistoryFilePath << std::endl;

//     return SingleParticleSimulatorInput( gravitationalParameter,
//                                          j2AcclerationModelFlag,
//                                          j2Coefficient,
//                                          equatorialRadius,
//                                          radiationPressureFlag,
//                                          particleRadius,
//                                          particleBulkDensity,
//                                          radiationPressureCoefficient,
//                                          initialStateKeplerianElements,
//                                          integrator,
//                                          startEpoch,
//                                          endEpoch,
//                                          timeStep,
//                                          relativeTolerance,
//                                          absoluteTolerance,
//                                          metadataFilePath,
//                                          stateHistoryFilePath );
// }

// } // namespace dustsim
