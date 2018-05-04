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
    DynamicalSystem(    const Vector3                  aAsymmetricBodyTorque, 
                        const Vector3                  aControlTorque,
                        const Vector3                  aDisturbanceTorque,
                        const Inertia                  aPrincipleInertia )
          : asymmetricBodyTorque( aAsymmetricBodyTorque ),  
            controlTorque( aControlTorque ),
            disturbanceTorque( aDisturbanceTorque ),
            principleInertia( aPrincipleInertia )
    { }

    void operator( )( const Vector7& state,
                      Vector7& stateDerivative,
                      const double time  )
    {
        Vector4 currentAttitude( state[0], state[1], state[2], state[3] ); 
        Vector3 currentAttitudeRate( state[4], state[5], state[6] );    

        Vector4 attitudeDerivative 
            = astro::computeQuaternionDerivative( currentAttitude, currentAttitudeRate );       

        Vector3 torque    = asymmetricBodyTorque + controlTorque + disturbanceTorque; 
 
        // std::cout << "the value of the torque is: " << torque << std::endl; 

        // Angular acceleration on the spacecraft. // 
        Vector3 acceleration; 
        acceleration[0]     = torque[0] / principleInertia[0]; 
        acceleration[1]     = torque[1] / principleInertia[1]; 
        acceleration[2]     = torque[2] / principleInertia[2];   

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

    //! Torque due to uneven principle moments of inertia. 
    const Vector3 asymmetricBodyTorque; 

    //! Torque due to control torque applied on the model
    const Vector3 controlTorque; 
    
    //! Torque on  the model due to disturbances. 
    const Vector3 disturbanceTorque;

    //! Principle inertia of the spacecraft. 
    const Inertia principleInertia; 
};

} // namespace dss_adcs

#endif // dss_adcs_DYNAMICAL_SYSTEM_HPP