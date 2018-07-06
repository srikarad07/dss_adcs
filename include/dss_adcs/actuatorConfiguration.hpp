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

    ActuatorConfiguration(  const std::vector< ReactionWheel > aReactionWheel )
                    : reactionWheel( aReactionWheel ) 
    { } 
    
    const std::pair< MatrixXd, MatrixXd > computeReactionWheelMappingMatrices( ) const
    {
        MatrixXd reactionWheelTorqueToControlTorqueMappingMatrix(3, reactionWheel.size()); 
        // TO DO: Need to test this function for accuracy. And in simulator convert the degree 
        // to radians before it is called here for wheelOrientation. 

        for ( unsigned int i = 0; i < reactionWheel.size(); ++i )
        {
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[0] = sin( reactionWheel[i].wheelOrientation[1] ) * cos( reactionWheel[i].wheelOrientation[0] );
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[1] = sin( reactionWheel[i].wheelOrientation[1] ) * sin( reactionWheel[i].wheelOrientation[0] );
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[2] = cos( reactionWheel[i].wheelOrientation[1] ); 
            // std::cout << "reaction Wheel wheel orientations: " << reactionWheel[i].wheelOrientation[0] << "," << reactionWheel[i].wheelOrientation[1] << std::endl; 
         }

        const MatrixXd inverseReactionWheelTorqueToControlTorqueMappingMatrix = reactionWheelTorqueToControlTorqueMappingMatrix.completeOrthogonalDecomposition().pseudoInverse();

        const std::pair< MatrixXd, MatrixXd > mappingMatrices( reactionWheelTorqueToControlTorqueMappingMatrix, inverseReactionWheelTorqueToControlTorqueMappingMatrix ); 
        
        return mappingMatrices;  
    }

    const Real calculateMassBudget() const
    {
        Real massBudget(0.0); 

        for ( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheel.size(); ++reactionWheelIterator )
        {
            massBudget += reactionWheel[reactionWheelIterator].mass;     
        }
        
        return massBudget; 
    }

    const Real calculateVolumeBudget( ) const
    {
        Real volumeBudget(0.0); 

        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheel.size(); ++reactionWheelIterator )
        {
            volumeBudget += reactionWheel[reactionWheelIterator].length * reactionWheel[reactionWheelIterator].height * reactionWheel[reactionWheelIterator].width; 
        }
        return volumeBudget; 
    }

    const VectorXd computeMaxReactionWheelTorque() const
    {
        VectorXd reactionWheelTorqueMax( reactionWheel.size() ); 
        
        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheel.size(); ++reactionWheelIterator )
        {
            reactionWheelTorqueMax[reactionWheelIterator]      = reactionWheel[reactionWheelIterator].maxTorque;
        }

        return reactionWheelTorqueMax; 
    }
    
protected: 

private: 

const std::vector< ReactionWheel > reactionWheel; 

}; // class 

} // namespace 

#endif 

