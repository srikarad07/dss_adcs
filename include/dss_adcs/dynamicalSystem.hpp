/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_DYNAMICAL_SYSTEM_HPP
#define DSS_ADCS_DYNAMICAL_SYSTEM_HPP

#include <iostream>
#include <math.h>
#include <map>

#include <Eigen/Dense>
#include <Eigen/LU>

#include <sml/sml.hpp>
#include <astro/astro.hpp>

#include "dss_adcs/typedefs.hpp"
#include "dss_adcs/controlTorque.hpp"
#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{

//! Class containing parameters and models to describe the dynamical system.
/*!
 * This class contains parameters and models to describe the dynamical system that governs the
 * rotational motion of a spacecraft. The dynamical system is defined in terms of a set of 1st-order ODEs,
 * with the right-hand side including all the force models acting on the spacecraft. This
 * dynamical system is provided as input to a numerical integrator to compute the derivative of the
 * state vector.
 */
class DynamicalSystem
{
public:
    // std::map< double, Vector3> controlTorqueMap;
    //! Construct dynamical system.
    /*!
     * Constructor for dynamical system, taking model parameters to define the dynamical
     * environment.
     *
     * @param[in] aGravitationalParameter   Gravitational parameter of the central body  [km^3 s^-2]
     * @param[in] aInertiaPrinciple         Princple axes of inertia's of the spacecraft [kg m^2]
     * @param[in] initial
     */
    DynamicalSystem( const Inertia                  aPrincipleInertia,
                     const Real                     aGravitationalParameter,
                     const Real                     aRadius,
                     const Real                     aSemiMajorAxis, 
                     const bool                     aGravityGradientAccelerationModelFlag,
                     const ActuatorConfiguration    anActuatorConfiguration,
                     const Vector3                  anAcceleration )
        : principleInertia( aPrincipleInertia ),
          gravitationalParameter( aGravitationalParameter ),
          radius( aRadius ),
          semiMajorAxis( aSemiMajorAxis ),
          gravityGradientAccelerationModelFlag( aGravityGradientAccelerationModelFlag ),
          actuatorConfiguration( anActuatorConfiguration ),
          acceleration( anAcceleration )
    { }

    void operator( )( const Vector7& state,
                      Vector7& stateDerivative,
                      const double time  )
    {
        Vector4 currentAttitude( state[0], state[1], state[2], state[3] ); 
        Vector3 currentAttitudeRate( state[4], state[5], state[6] );    

        Vector4 attitudeDerivative 
            = astro::computeQuaternionDerivative( currentAttitude, currentAttitudeRate );       
                     
        // Set the derivative of the velocity elements to the computed total acceleration.
        stateDerivative[ 0 ] = attitudeDerivative[ 0 ];
        stateDerivative[ 1 ] = attitudeDerivative[ 1 ];
        stateDerivative[ 2 ] = attitudeDerivative[ 2 ];
        stateDerivative[ 3 ] = attitudeDerivative[ 3 ];
        stateDerivative[ 4 ] = acceleration[ 0 ];
        stateDerivative[ 5 ] = acceleration[ 1 ];
        stateDerivative[ 6 ] = acceleration[ 2 ];

    }

protected:

private:

    //! Principle Inertia of the orbiting body [km m^2].
    const Inertia principleInertia;

    //! Gravitational parameter of the central body [km^3 s⁻2].gravitationalParameter
    const Real gravitationalParameter;

    //! Magnitude of the radial vector of the orbiting body from the central body.
    const Real radius;

    //! Radius of the spacecraft around the orbit. 
    const Real semiMajorAxis;

    //! Gravity gradient acceleration disturbance model flag. 
    const bool gravityGradientAccelerationModelFlag;

    //! Actuator configuration for concept generation. 
    const ActuatorConfiguration actuatorConfiguration;

    //! Acceleration
    const Vector3 acceleration; 
};

} // namespace dss_adcs

#endif // dss_adcs_DYNAMICAL_SYSTEM_HPP