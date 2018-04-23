/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef CONTROL_TORQUE_HPP
#define CONTROL_TORQUE_HPP

#include "astro/quaternionFeedbackControl.hpp"
#include "dss_adcs/reactionWheelConcept.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{

// Compute the value of control torque 
template < typename Vector3, typename Vector4 >
Vector3 computeRealTorqueValue(     const Vector4 quaternionCurrent, 
                                    const Vector3 angularVelocity, 
                                    const Vector3 quaternionControlGainMatrix, 
                                    const Vector3 angularVelocityControlGainMatrix )
{
// <<<<<<<<<<<<<<<<<<<<<<<<< Assumptions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
// Quaternion reference is just taken as a single value, which is not necessarily true //     
Vector4 quaternionReference( 0.0, 0.0, 0.0, 1.0 ); 
// <<<<<<<<<<<<<<<<<<<<<<<<< End of Assumptions >>>>>>>>>>>>>>>>>>>>>>> // 

Vector3 controlTorque   = astro::computeQuaternionControlTorque( quaternionReference, 
                                                                 quaternionCurrent, 
                                                                 angularVelocity, 
                                                                 quaternionControlGainMatrix, 
                                                                 angularVelocityControlGainMatrix ); 
                                                                 
ReactionWheel rw1, rw2, rw3; 
// rw1.mass   = 1.2; 
rw1.maxTorque = 2.0; 
rw2.maxTorque = 2.5; 
rw3.maxTorque = 3.0; 

Concept AxisConcept( rw1, rw2, rw3 ); 

Vector3 torque = AxisConcept(); 

// std::cout << "Trial concept torque: " << torque << std::endl; 

return controlTorque; 

}  //template 

} // namespace astro 

#endif // CONTROL_TORQUE_HPP