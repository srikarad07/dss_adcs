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
                                                                    const ActuatorConfiguration&    actuatorConfiguration, 
                                                                    const std::string               controllerType, 
                                                                    const Real                      naturalFrequency, 
                                                                    const Real                      dampingRatio, 
                                                                    const Real                      slewRateSaturation, 
                                                                    const Vector3                   principleInertiaVector, 
                                                                    const Vector4                   initialQuaternion,
                                                                    const Real                      simulationTime ) 
{
    //! Maximum reaction wheel torque. 
    const VectorXd reactionWheelTorqueMax = actuatorConfiguration.computeMaxReactionWheelTorque();

    const Matrix33 principleInertiaMatrix           = principleInertiaVector.asDiagonal(); 
    const Real angularVelocityControlGain           = 2 * naturalFrequency * dampingRatio;
    const Real k = 2 * naturalFrequency * naturalFrequency;

    const Matrix33 angularVelocityControlGainMatrix = angularVelocityControlGain * principleInertiaMatrix;
    const Matrix33 attitudeControlGainMatrix        =  k * principleInertiaMatrix; 

    //! Compute the reaction wheel mapping matrices. 
    const std::pair< MatrixXd, MatrixXd > mappingMatrices = actuatorConfiguration.computeReactionWheelMappingMatrices( );
    const MatrixXd reactionWheelTorqueToControlTorqueMappingMatrix = mappingMatrices.first; 
    const MatrixXd inverseReactionWheelTorqueToControlTorqueMappingMatrix = mappingMatrices.second; 

    Vector3 commandedControlTorque;

    if ( controllerType.compare("linear") == 0)
    {
        commandedControlTorque   = astro::computeQuaternionControlTorque( quaternionReference, 
                                                                          quaternionCurrent, 
                                                                          angularVelocity, 
                                                                          attitudeControlGainMatrix, 
                                                                          angularVelocityControlGainMatrix  );     
    }

    else if ( controllerType.compare("cascade_saturation") == 0 )
    {
        commandedControlTorque = astro::computeControlTorqueWithSaturationCascadeController( naturalFrequency, 
                                                                                    dampingRatio, 
                                                                                    slewRateSaturation, 
                                                                                    principleInertiaVector, 
                                                                                    initialQuaternion, 
                                                                                    quaternionReference, 
                                                                                    quaternionCurrent, 
                                                                                    angularVelocity, 
                                                                                    simulationTime );
    }

    else
    {
        std::cout << "Controller " <<  controllerType << " not implemented! " << std::endl;
        throw; 
    }

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