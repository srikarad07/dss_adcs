/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_ACTUATOR_CONFIGURATION_HPP
#define DSS_ADCS_ACTUATOR_CONFIGURATION_HPP

#include <math.h> 

#include <Eigen/QR>    

#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{          

class ActuatorConfiguration
{
public: 

    // VectorXd reactionWheelMotorTorque(reactionWheel.size());
    Vector3 reactionWheelMotorTorque;

    ActuatorConfiguration(  const std::vector< ReactionWheel > aReactionWheel, 
                            const std::vector< Vector2 > aWheelOrientation )
                    : reactionWheel( aReactionWheel ), 
                      wheelOrientation( aWheelOrientation )   
    { }
    
    Vector3 computePrincipleAxesTorque ( const Vector3 controlTorque ) 
    {
        // Update this to the reaction wheel orientation matrix to convert the RW torques to 
        // principle axes torques.
        Vector3 reactionWheelTorqueMax; 
        // std::cout << reactionWheel[0].maxTorque << std::endl; 
        if ( reactionWheel.size() != wheelOrientation.size() )
        {
            std::cout << "The number of reaction wheel and the given wheel orientations are not equal" << std::endl; 
            throw; 
        }

        MatrixXd reactionWheelTorqueToControlTorqueMappingMatrix(3, reactionWheel.size()); 
        // TO DO: Need to test this function for accuracy. And in simulator convert the degree 
        // to radians before it is called here for wheelOrientation. 

        for ( unsigned int i = 0; i < reactionWheel.size(); ++i )
        {
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[0] = sin( wheelOrientation[i][1] ) * cos( wheelOrientation[i][0] );
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[1] = sin( wheelOrientation[i][1] ) * sin( wheelOrientation[i][0] );
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[2] = cos( wheelOrientation[i][1] ); 

            // Generate torque from the reaction wheel. 
            reactionWheelTorqueMax[i]      = reactionWheel[i].maxTorque; 
        }
        // std::cout << "Torques: "<< reactionWheelTorqueMax << std::endl; 
        MatrixXd inverseReactionWheelTorqueToControlTorqueMappingMatrix(reactionWheel.size(), 3); 
        inverseReactionWheelTorqueToControlTorqueMappingMatrix = reactionWheelTorqueToControlTorqueMappingMatrix.completeOrthogonalDecomposition().pseudoInverse();
        
        reactionWheelMotorTorque     = inverseReactionWheelTorqueToControlTorqueMappingMatrix * controlTorque; 

        for ( unsigned int iterator = 0; iterator < reactionWheelMotorTorque.size(); ++iterator )
        {
            if ( reactionWheelTorqueMax.array().abs()[iterator] < reactionWheelMotorTorque.array().abs()[iterator] )
            {
                Real errorSign        = dss_adcs::signFunction( reactionWheelMotorTorque[ iterator ] ); 
                reactionWheelMotorTorque[iterator]  = errorSign*reactionWheelTorqueMax[iterator]; 
            }   
        }

        Vector3 controlTorque1   = reactionWheelTorqueToControlTorqueMappingMatrix * reactionWheelMotorTorque; 
        
        // Vector3 controlTorque; 
        // controlTorque[0]    = reactionWheelTorqueToControlTorqueMappingMatrix.col(0)[0] * reactionWheelTorque[0] + reactionWheelTorqueToControlTorqueMappingMatrix.col(1)[0] * reactionWheelTorque[1] + reactionWheelTorqueToControlTorqueMappingMatrix.col(2)[0] * reactionWheelTorque[2]; 

        // controlTorque[1]    = reactionWheelTorqueToControlTorqueMappingMatrix.col(0)[1] * reactionWheelTorque[0] + reactionWheelTorqueToControlTorqueMappingMatrix.col(1)[1] * reactionWheelTorque[1] + reactionWheelTorqueToControlTorqueMappingMatrix.col(2)[1] * reactionWheelTorque[2]; 

        // controlTorque[2]    = reactionWheelTorqueToControlTorqueMappingMatrix.col(0)[2] * reactionWheelTorque[0] + reactionWheelTorqueToControlTorqueMappingMatrix.col(1)[2] * reactionWheelTorque[1] + reactionWheelTorqueToControlTorqueMappingMatrix.col(2)[2] * reactionWheelTorque[2]; 

        return controlTorque1;     
    }
    
protected: 

private: 

const std::vector< ReactionWheel > reactionWheel; 

const std::vector< Vector2 > wheelOrientation;  

}; // class 

} // namespace 

#endif 

