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

Vector3 feasibleContolTorque = actuatorConfiguration.computePrincipleAxesTorque( commandedControlTorque );  
return feasibleContolTorque; 

}  //template 

} // namespace astro 

#endif // CONTROL_TORQUE_HPP