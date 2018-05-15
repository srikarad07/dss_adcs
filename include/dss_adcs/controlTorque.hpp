/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef CONTROL_TORQUE_HPP
#define CONTROL_TORQUE_HPP

#include <math.h> 

#include <Eigen/Dense>

#include "dss_adcs/actuatorConfiguration.hpp"
#include "astro/quaternionFeedbackControl.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{

// Compute the value of control torque 
template < typename Vector3, typename Vector4 >
Vector3 computeRealTorqueValue(     const Vector4                   quaternionCurrent, 
                                    const Vector4                   quaternionReference,
                                    const Vector3                   angularVelocity, 
                                    const Vector3                   quaternionControlGainMatrix, 
                                    const Vector3                   angularVelocityControlGainMatrix,
                                    const ActuatorConfiguration&    actuatorConfiguration    )
{

Vector3 controlTorque   = astro::computeQuaternionControlTorque( quaternionReference, 
                                                                 quaternionCurrent, 
                                                                 angularVelocity, 
                                                                 quaternionControlGainMatrix, 
                                                                 angularVelocityControlGainMatrix );                           

const Vector3 realTorque = actuatorConfiguration.computePrincipleAxesTorque( controlTorque ); 

// <<<<<<<<<<<<<<<<<<<<<<<<< For reaction wheels >>>>>>>>>>>>>>>>>>>>>>       //
// The torque of the reaction wheel is in the range -maxTorque to + maxTorque //  
// if ( controlTorque.array().abs()[0] > torque.array().abs()[0]  )
// {
//     std::cout << "Not enough torque: " << std::endl; 
//     controlTorque[0]       = torque[0]; 
// }

// if ( controlTorque.array().abs()[1] > torque.array().abs()[1]  )
// {
//     controlTorque[1]       = torque[1]; 
// }

// if ( controlTorque.array().abs()[2] > torque.array().abs()[2]  )
// {
//     controlTorque[2]       = torque[2]; 
// }

// // std::cout << controlTorque[0] << std::endl; 
return realTorque; 

}  //template 

} // namespace astro 

#endif // CONTROL_TORQUE_HPP