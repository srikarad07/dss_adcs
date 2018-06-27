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
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{

inline const std::pair< Vector3, VectorXd > computeRealTorqueValue( const Vector4                   quaternionCurrent, 
                                                                    const Vector4                   quaternionReference,
                                                                    const Vector3                   angularVelocity, 
                                                                    const Real                      quaternionControlGain, 
                                                                    const Vector3                   angularVelocityControlGainMatrix,
                                                                    const ActuatorConfiguration&    actuatorConfiguration ) 
{
    const Vector3 commandedControlTorque   = astro::computeQuaternionControlTorque( quaternionReference, 
                                                                                    quaternionCurrent, 
                                                                                    angularVelocity, 
                                                                                    quaternionControlGain, 
                                                                                    angularVelocityControlGainMatrix );                           

    const VectorXd reactionWheelTorqueMax = actuatorConfiguration.computeMaxReactionWheelTorque(); 

    const std::pair< MatrixXd, MatrixXd > mappingMatrices = actuatorConfiguration.computeReactionWheelMappingMatrices( );

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

    const Vector3 controlTorque   = reactionWheelTorqueToControlTorqueMappingMatrix * reactionWheelMotorTorque; 

    const std::pair< Vector3, VectorXd > outputTorques( controlTorque, reactionWheelMotorTorque ); 
    
    return outputTorques; 
}  //template 

} // namespace astro 

#endif // CONTROL_TORQUE_HPP