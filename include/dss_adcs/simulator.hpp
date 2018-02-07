/*
 * Copyright (c) 2018 S.D. Cherukuri (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_FACTORIAL_HPP
#define DSS_ADCS_FACTORIAL_HPP

#include "dss_adcs/typedefs.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace dss_adcs
{

void executeSimulator( const rapidjson::Document& config );
// void executeSimulator( const rapidjson::Document& config );
struct simulatorInput
{
public:
    simulatorInput( const Real            aPrincipleInertia )     
        : principleInertia( aPrincipleInertia )
    { }

    //! Gravitational parameter of central body [km^3 s^-2].
    const Real principleInertia;

protected:

private:
};

simulatorInput checkSimulatorInput( const rapidjson::Document& config );
//! Compute factorial.
/*!
 * Computes factorial of a given integer.
 *
 * @param  integerNumber Integer number to compute factorial of
 * @return               Value of factorial of integer
 */
const int computeFactorial( const int integerNumber );

} // namespace dss_adcs

#endif // DSS_ADCS_FACTORIAL_HPP
