/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef CONTROL_TORQUE_HPP
#define CONTROL_TORQUE_HPP

#include "astro/quaternionFeedbackControl.hpp"

namespace dss_adcs
{

// Compute the value of control torque 
template < typename Vector3, typename Vector4 >
Vector3 computeRealTorqueValue(     const Vector4 quaternionCurrent, 
                                    const Vector3 angularVelocity, 
                                    const Vector3 quaternionControlGainMatrix, 
                                    const Vector3 angularVelocityControlGainMatrix )
{
Vector4 quaternionReference( 10.0, 10.0, 10.0, 10.0 ); 

Vector3 controlTorque   = astro::computeQuaternionControlTorque( quaternionReference, 
                                                                 quaternionCurrent, 
                                                                 angularVelocity, 
                                                                 quaternionControlGainMatrix, 
                                                                 angularVelocityControlGainMatrix ); 

// std::cout << "The controlTorque is: " << controlTorque << std::endl; 
return controlTorque; 

}  //template 

} // namespace astro 

#endif // CONTROL_TORQUE_HPP