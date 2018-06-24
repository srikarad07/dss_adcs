/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef CONTROL_TORQUE_HPP
#define CONTROL_TORQUE_HPP

#include <math.h> 

#include <Eigen/Dense>

#include "astro/quaternionFeedbackControl.hpp"

#include "dss_adcs/actuatorConfiguration.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{

// Compute the value of control torque 
template < typename Vector3, typename Vector4, typename Real >
Vector3 computeRealTorqueValue(     const Vector4                   quaternionCurrent, 
                                    const Vector4                   quaternionReference,
                                    const Vector3                   angularVelocity, 
                                    const Real                      quaternionControlGain, 
                                    const Vector3                   angularVelocityControlGainMatrix,
                                    ActuatorConfiguration&          actuatorConfiguration    )
{

Vector3 commandedControlTorque   = astro::computeQuaternionControlTorque( quaternionReference, 
                                                                          quaternionCurrent, 
                                                                          angularVelocity, 
                                                                          quaternionControlGain, 
                                                                          angularVelocityControlGainMatrix );                           

    VectorXd reactionWheelTorqueMax = actuatorConfiguration.computeMaxReactionWheelTorque(); 

    std::pair< MatrixXd, MatrixXd > mappingMatrices = actuatorConfiguration.computeReactionWheelMappingMatrices( );

    const MatrixXd reactionWheelTorqueToControlTorqueMappingMatrix = mappingMatrices.first; 
    const MatrixXd inverseReactionWheelTorqueToControlTorqueMappingMatrix = mappingMatrices.second; 

    VectorXd reactionWheelMotorTorque     = inverseReactionWheelTorqueToControlTorqueMappingMatrix * commandedControlTorque;  
    
    for ( unsigned int iterator = 0; iterator < reactionWheelMotorTorque.size(); ++iterator )
    {
        if ( reactionWheelTorqueMax.array().abs()[iterator] < reactionWheelMotorTorque.array().abs()[iterator] )
        {
            Real errorSign        = dss_adcs::signFunction( reactionWheelMotorTorque[ iterator ] ); 
            reactionWheelMotorTorque[iterator]  = errorSign*reactionWheelTorqueMax[iterator]; 
        }   
    }

    Vector3 controlTorque   = reactionWheelTorqueToControlTorqueMappingMatrix * reactionWheelMotorTorque; 

    return controlTorque; 

}  //template 

} // namespace astro 

#endif // CONTROL_TORQUE_HPP