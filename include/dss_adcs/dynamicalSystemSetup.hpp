/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DYNAMICAL_SYSTEM_SETUP_HPP
#define DYNAMICAL_SYSTEM_SETUP_HPP

#include <Eigen/Dense>

#include <astro/astro.hpp>

#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/typedefs.hpp"
#include "dss_adcs/controlTorque.hpp"

namespace dss_adcs
{
class DynamicalSystemSetup 
{
public: 

    DynamicalSystemSetup(   const State                     aCurrentState, 
                            const Real                      aTimeStep, 
                            const Inertia                   aPrincipleInertia,
                            const Real                      anIntegrationStartTime, 
                            const ActuatorConfiguration     anActuatorConfiguration     )
                : currentState( aCurrentState ),
                  timeStep( aTimeStep ),
                  principleInertia( aPrincipleInertia ),
                  integrationStartTime( anIntegrationStartTime ),
                  actuatorConfiguration( anActuatorConfiguration )
            { }
    
    Real        integrationEndTime; 
    Vector3     controlTorque; 

    Vector3 computeAngularAcceleration( )
    {
        // End time for the integration. 
        integrationEndTime = integrationStartTime + timeStep; 

        Vector4 currentAttitude( currentState[0], currentState[1], currentState[2], currentState[3] ); 
        Vector3 currentAttitudeRate( currentState[4], currentState[5], currentState[6] ); 

        Vector3 torque = astro::computeRotationalBodyAcceleration( principleInertia, currentAttitudeRate );

        // Disturbance torques. 
        // if ( gravityGradientAccelerationModelFlag == true )
        // {
        //        Matrix33 directionCosineMatrix( astro::computeEulerAngleToDcmConversionMatrix(rotationSequence, currentAttitude) );

        //        acceleration += astro::computeGravityGradientTorque( gravitationalParameter, radius, principleInertia, directionCosineMatrix ); 
 
        // }

        // Control torque on the dynamics: 
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<< ASSUMPTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
        Vector3 quaternionControlGainMatrix( 10.0, 10.0, 10.0);
        Vector3 angularVelocityControlGainMatrix( 10.0, 10.0, 10.0); 
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<  End of assumptions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
        // TO DO: check the controller applicability. Nonlinearity of the equations as well need to be checked //
        
        controlTorque = dss_adcs::computeRealTorqueValue(   currentAttitude, 
                                                            currentAttitudeRate, 
                                                            quaternionControlGainMatrix, 
                                                            angularVelocityControlGainMatrix, 
                                                            actuatorConfiguration );

        torque  += controlTorque; 

        // Angular acceleration on the spacecraft is calculated as. // 
        Vector3 acceleration; 
        acceleration[0]     = torque[0] / principleInertia[0]; 
        acceleration[1]     = torque[1] / principleInertia[1]; 
        acceleration[2]     = torque[2] / principleInertia[2];   

        return acceleration; 
    }
protected: 

private: 

    //! Current quaternion state of the attitude state
    const State currentState; 

    //! Integration timestep. 
    const Real timeStep; 

    //! Principle inertia at the body axes of the body. 
    const Inertia principleInertia; 

    //! Start time of the integration. 
    const Real integrationStartTime; 

    //! Configuration of the actuators in the concepts. 
    const ActuatorConfiguration actuatorConfiguration; 

};

}

#endif