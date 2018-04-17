/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_DYNAMICAL_SYSTEM_HPP
#define DSS_ADCS_DYNAMICAL_SYSTEM_HPP

#include <iostream>
// #include <conio.h>
#include <math.h>

#include <Eigen/Dense>
#include <Eigen/LU>

#include <sml/sml.hpp>
#include <astro/astro.hpp>

#include "dss_adcs/typedefs.hpp"

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

    //! Construct dynamical system.
    /*!
     * Constructor for dynamical system, taking model parameters to define the dynamical
     * environment.
     *
     * @param[in] aGravitationalParameter   Gravitational parameter of the central body  [km^3 s^-2]
     * @param[in] aInertiaPrinciple         Princple axes of inertia's of the spacecraft [kg m^2]
     * @param[in] intial
     */
    DynamicalSystem( const Inertia      aPrincipleInertia,
                     const Real         aGravitationalParameter,
                     const Real         aRadius,
                     const Real         aSemiMajorAxis, 
                     const bool         aGravityGradientAcclerationModelFlag )
        : principleInertia( aPrincipleInertia ),
          gravitationalParameter( aGravitationalParameter ),
          radius( aRadius ),
          semiMajorAxis( aSemiMajorAxis ),
          gravityGradientAcclerationModelFlag( aGravityGradientAcclerationModelFlag )
    { }

    //! Overload ()-operator to compute state derivative using dynamical system.
    /*!
     * Overloads the ()-operator to compute the state derivative, given the current state and epoch,
     * based on the parameters and models used to construct the dynamical system.
     * This function fulfills the prototype for the numerical integrators in the Boost Odeint
     * library.
     *
     * @sa boost::odeint::integrator
     * @param[in]  state             Current state of the dynamical system (1-D vector)
     * @param[out] stateDerivative   Computed state derivative of the dynamical system (1-D vector)
     * @param[in]  time              Current simulation epoch
     */
    // void operator( )( const Vector6& state,
    //                   Vector6& stateDerivative,
    //                   const double time  )
    // {
    //     Position currentAttitude( state[0], state[1], state[2] ); 
    //     Velocity currentAttitudeRate( state[3], state[4], state[5] );    
    //     Matrix33 rotationSequence;
    //     rotationSequence << 0, 0, 1, 
    //                         0, 1, 0, 
    //                         0, 0, 1; 
        
    //     Real meanAngularMotion             = pow( gravitationalParameter/(pow(semiMajorAxis,3)), 0.5);

    //     Vector3 attitudeDerivative 
    //         = astro::eulerKinematicDifferential( rotationSequence, currentAttitude, currentAttitudeRate, meanAngularMotion );

    //     // Compute the total acceleration acting on the system as a sum of the forces.
    //     // Central body gravity is included by default.
    //     Vector3 acceleration
    //         = astro::computeRotationalBodyAcceleration( principleInertia, currentAttitudeRate );
    //     // std::cout << "The acceleration is: " << acceleration[0] << std::endl; 
    //     // sml::add( acceleration, dss_adcs::computeGravityGradientTorque( gravitationalParameter, radius, principleInertia, directionCosineMatrix ) );
    //     if ( gravityGradientAcclerationModelFlag == true )
    //     {
    //            Matrix33 directionCosineMatrix( astro::computeEulerAngleToDcmConversionMatrix(rotationSequence, currentAttitude) );

    //            acceleration += astro::computeGravityGradientTorque( gravitationalParameter, radius, principleInertia, directionCosineMatrix ); 
    //         //    std::cout << "Gravity gradient acceleration disturbance model is active" << std::endl; 
    //     }    
    //     // std::cout << "The acceleration gravity gradient is: " << temp_acceleration[0] << std::endl; 
    //     // Set the derivative of the velocity elements to the computed total acceleration.
    //     stateDerivative[ 0 ] = attitudeDerivative[ 0 ];
    //     stateDerivative[ 1 ] = attitudeDerivative[ 1 ];
    //     stateDerivative[ 2 ] = attitudeDerivative[ 2 ];
    //     stateDerivative[ 3 ] = acceleration[ 0 ];
    //     stateDerivative[ 4 ] = acceleration[ 1 ];
    //     stateDerivative[ 5 ] = acceleration[ 2 ];

    // }

    void operator( )( const Vector7& state,
                      Vector7& stateDerivative,
                      const double time  )
    {
        // Quaterniond currentAttitude( state[3], state[0], state[1], state[2]); 
        Vector4 currentAttitude( state[0], state[1], state[2], state[3] ); 
        Vector3 currentAttitudeRate( state[4], state[5], state[6] );    

        Vector4 attitudeDerivative 
            = astro::computeQuaternionDerivative( currentAttitude, currentAttitudeRate );
        // std::cout << "The attitude derivative: \n" << attitudeDerivative << std::endl; 
        // Compute the total acceleration acting on the system as a sum of the forces.
        Vector3 acceleration
            = astro::computeRotationalBodyAcceleration( principleInertia, currentAttitudeRate );
        // std::cout << "The acceleration of \n" << acceleration << std::endl; 
        // if ( gravityGradientAcclerationModelFlag == true )
        // {
        //        Matrix33 directionCosineMatrix( astro::computeEulerAngleToDcmConversionMatrix(rotationSequence, currentAttitude) );

        //        acceleration += astro::computeGravityGradientTorque( gravitationalParameter, radius, principleInertia, directionCosineMatrix ); 
 
        // }
        // Vector4 attitudeDerivative; 
        // attitudeDerivative << 0.0, 0.0, 0.0, 0.0; 
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

    //! Gravity graditen acceleration disturbance model flag. 
    const bool gravityGradientAcclerationModelFlag;

};

} // namespace dss_adcs

#endif // dss_adcs_DYNAMICAL_SYSTEM_HPP