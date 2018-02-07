/*
 * Copyright (c) 2018 S.D. Cherukuri (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <fstream>
#include <iostream>
#include <stdexcept>

#include <boost/numeric/odeint.hpp>

// #include <astro/astro.hpp>

// #include "dss_adcs/dynamicalSystem.hpp"
#include "dss_adcs/simulator.hpp"

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

    // Compute initial state in Cartesian elements.
    // State initialState = astro::convertKeplerianToCartesianElements(
    //     input.initialStateKeplerianElements, input.gravitationalParameter );
    // std::cout << "Cartesian initial state            (";
    // for ( unsigned int i = 0; i < initialState.size( ) - 1; i++ )
    // {
    //     std::cout << initialState[ i ] << ", ";
    // }
    // std::cout << initialState[ initialState.size( ) - 1 ] << ")" << std::endl;
    // std::cout << std::endl;
};

simulatorInput checkSimulatorInput( const rapidjson::Document& config )
{
    const Real principleInertia
        = 50.0;
    std::cout << "Principle Inertia            " << principleInertia
              << " [kg m^2]" << std::endl;         
    
    return simulatorInput( principleInertia );
};

//! Compute factorial.
const int computeFactorial( const int integerNumber )
{
	if ( integerNumber < 0 )
	{
		std::ostringstream errorMessage;
		errorMessage << "ERROR: (" << integerNumber << "!) is undefined!" << std::endl;
		throw std::runtime_error( errorMessage.str( ) );
	}

	return ( integerNumber == 0 ) ? 1 : integerNumber * computeFactorial( integerNumber  - 1 );
};

} // namespace dss_adcs
