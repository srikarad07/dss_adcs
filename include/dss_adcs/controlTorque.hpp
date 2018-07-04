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
                                                                    const ActuatorConfiguration&    actuatorConfiguration, 
                                                                    const std::string               controllerType, 
                                                                    const Real                      naturalFrequency, 
                                                                    const Real                      dampingRatio, 
                                                                    const Real                      slewRateSaturation, 
                                                                    const Vector3                   principleInertiaVector, 
                                                                    const Vector4                   initialQuaternion,
                                                                    const Real                      simulationTime ) 
                                                                    // const Real                      initialTime=0.0,
                                                                    // const Real                      finalTime=2000.0,
                                                                    // const Real                      timeStep=1.0 ) 
{
    //! Maximum reaction wheel torque. 
    // const VectorXd reactionWheelTorqueMax = actuatorConfiguration.computeMaxReactionWheelTorque();
    const Vector4 tempvector( 0.3, 0.3, 0.3, 0.3 );  
    const VectorXd reactionWheelTorqueMax = tempvector; 
    //! Compute the reaction wheel mapping matrices. 
    const std::pair< MatrixXd, MatrixXd > mappingMatrices = actuatorConfiguration.computeReactionWheelMappingMatrices( );
    const MatrixXd reactionWheelTorqueToControlTorqueMappingMatrix = mappingMatrices.first; 
    const MatrixXd inverseReactionWheelTorqueToControlTorqueMappingMatrix = mappingMatrices.second; 
    
    // std::cout << "Mappping matrice reation wheel to control torques: " << reactionWheelTorqueToControlTorqueMappingMatrix << std::endl;
    // std::cout << "Mappping matrice inverse: " << inverseReactionWheelTorqueToControlTorqueMappingMatrix << std::endl;
    
    // const Vector3 maxControlTorque = reactionWheelTorqueToControlTorqueMappingMatrix * reactionWheelTorqueMax; 
    
    // std::cout<< "reaction Wheel torques maximum values: " << reactionWheelTorqueMax << std::endl;  
    // std::cout << "Maximum control input on all three axes: " << maxControlTorque << std::endl; 
    // Vector3 controlTorque(0.0, 0.0, 0.0); 
    // VectorXd reactionWheelMotorTorque; 
    Vector3 commandedControlTorque;

    if ( controllerType.compare("linear") == 0)
    {
        commandedControlTorque   = astro::computeQuaternionControlTorque( quaternionReference, 
                                                                                        quaternionCurrent, 
                                                                                        angularVelocity, 
                                                                                        quaternionControlGain, 
                                                                                        angularVelocityControlGainMatrix );     
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
                                                                                    // initialTime, 
                                                                                    // finalTime, 
                                                                                    // timeStep );

        // controlTorque = astro::normalizedSaturationFunction( commandedControlTorque, maxControlTorque ); 
        // reactionWheelMotorTorque = inverseReactionWheelTorqueToControlTorqueMappingMatrix * controlTorque; 
        // reactionWheelMotorTorque = inverseReactionWheelTorqueToControlTorqueMappingMatrix * controlTorque;  
    }

    else
    {
        std::cout << "Controller " <<  controllerType << " not implemented! " << std::endl;
        throw; 
    }

    // std::cout << "Commanded control torque: " << commandedControlTorque << std::endl; 
    // const Vector3 controlTorque = astro::normalizedSaturationFunction( commandedControlTorque, maxControlTorque ); 
    // const VectorXd reactionWheelMotorTorque = inverseReactionWheelTorqueToControlTorqueMappingMatrix * controlTorque; 
    VectorXd reactionWheelMotorTorque     = inverseReactionWheelTorqueToControlTorqueMappingMatrix * commandedControlTorque;  
    std::cout << "reaction wheel motor torque: " << reactionWheelMotorTorque << std::endl;
    for ( unsigned int iterator = 0; iterator < reactionWheelMotorTorque.size(); ++iterator )
    {
        if ( reactionWheelTorqueMax.array().abs()[iterator] < reactionWheelMotorTorque.array().abs()[iterator] )
        {
            Real errorSign        = dss_adcs::signFunction( reactionWheelMotorTorque[ iterator ] ); 
            reactionWheelMotorTorque[iterator]  = errorSign*reactionWheelTorqueMax[iterator]; 
        }   
    }
    // std::cout << "Reaction wheel motor torque after saturation: " << reactionWheelMotorTorque << std::endl; 
    const Vector3 controlTorque   = reactionWheelTorqueToControlTorqueMappingMatrix * reactionWheelMotorTorque; 
    // std::cout << "Maping matrix: " << reactionWheelTorqueToControlTorqueMappingMatrix << std::endl;
    // std::cout << "Contorl torque: " << controlTorque << std::endl; 
    const std::pair< Vector3, VectorXd > outputTorques( controlTorque, reactionWheelMotorTorque ); 
    
    return outputTorques; 
}  //template 

} // namespace astro 

#endif // CONTROL_TORQUE_HPP