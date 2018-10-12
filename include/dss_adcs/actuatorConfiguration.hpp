/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_ACTUATOR_CONFIGURATION_HPP
#define DSS_ADCS_ACTUATOR_CONFIGURATION_HPP

#include <cmath>
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
        // TO DO: Need to test this function for accuracy.

        for ( unsigned int i = 0; i < reactionWheel.size(); ++i )
        {
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[0] = sin( reactionWheel[i].wheelOrientation[1] ) * cos( reactionWheel[i].wheelOrientation[0] );
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[1] = sin( reactionWheel[i].wheelOrientation[1] ) * sin( reactionWheel[i].wheelOrientation[0] );
            reactionWheelTorqueToControlTorqueMappingMatrix.col(i)[2] = cos( reactionWheel[i].wheelOrientation[1] ); 
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
            if ( !isnan( reactionWheel[reactionWheelIterator].radius ) ) 
            {
                volumeBudget += sml::SML_PI * reactionWheel[reactionWheelIterator].radius * reactionWheel[reactionWheelIterator].radius * reactionWheel[reactionWheelIterator].height; 
                
            }
            else if ( !isnan( reactionWheel[reactionWheelIterator].length) && !isnan( reactionWheel[reactionWheelIterator].width) )
            {
                volumeBudget += reactionWheel[reactionWheelIterator].length * reactionWheel[reactionWheelIterator].height * reactionWheel[reactionWheelIterator].width;
            }
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

    const VectorXd calculateMomentOfInertia( ) const 
    {
        VectorXd reactionWheelMomentsOfInertia( reactionWheel.size() );

        for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheel.size(); ++reactionWheelIterator )
        {
            if ( !isnan( reactionWheel[reactionWheelIterator].radius ) ) 
            {
                // std::cout << "Enters the first loop! " << std::endl; 
                reactionWheelMomentsOfInertia[reactionWheelIterator] = reactionWheel[reactionWheelIterator].mass * reactionWheel[reactionWheelIterator].radius * reactionWheel[reactionWheelIterator].radius / 2.0; 
            }
            else if ( !isnan( reactionWheel[reactionWheelIterator].length) && !isnan( reactionWheel[reactionWheelIterator].width) )
            {
                reactionWheelMomentsOfInertia[reactionWheelIterator] = reactionWheel[reactionWheelIterator].mass * ( ( reactionWheel[reactionWheelIterator].length * reactionWheel[reactionWheelIterator].length ) + ( reactionWheel[reactionWheelIterator].width * reactionWheel[reactionWheelIterator].width ) ) / 12.0; 
                // std::cout << "Enters this loop" << std::endl; 
                // reactionWheelMomentsOfInertia[reactionWheelIterator] = reactionWheel[reactionWheelIterator].mass * reactionWheel[reactionWheelIterator].length * reactionWheel[reactionWheelIterator].length / 8.0; 
            }
        } 
        return reactionWheelMomentsOfInertia; 
    }

    const VectorXd computeReactionWheelVelocities( const VectorXd reactionWheelAngularMomentum ) const 
    {
        VectorXd reactionWheelAngularVelocities( reactionWheelAngularMomentum.size() );
        VectorXd momentsOfInertiaVector = calculateMomentOfInertia(); 
        // std::cout << "The moment of inertia: " << momentsOfInertiaVector << std::endl;
        // std::cout << "Angular momentum: " << reactionWheelAngularMomentum << std::endl; 
        for ( unsigned int iterator = 0; iterator < reactionWheelAngularMomentum.size(); ++iterator )
        {
            reactionWheelAngularVelocities[iterator] = reactionWheelAngularMomentum[iterator] / momentsOfInertiaVector[iterator]; 
        }  

        return reactionWheelAngularVelocities; 
    }

    //! Reaction wheel power consumption is calculated as an estimate with formula given below: 
    /* Reaction wheel power consumption given as an estimate for reaction wheel after estimating it 
     * for 20 reaction wheels in "Comparison of Control Moment Gyros and Reaction Wheels for small 
     * Earth observing satellites" (https://digitalcommons.usu.edu/cgi/viewcontent.cgi?article=1080&context=smallsat).      
     *   \f[
     *         P_{RW} = 1000T + 4.51h^{0.47}  
     *   \f]  
     * where $P_{RW}$ is the power consumed by the reaction wheel in $W$, $T$ is the motor torque acting on the 
     * reaction wheel in $N-m$ and $h$ is the angular momentum of the reaction wheel in $Nm-sec$. Power values
     * taken as positive.
     *  
     *   @tparam         VectorXd                    x dimensional-vector type
     *   @param[in]      VectorXd                    Reaction wheel motor torque [N-m] 
     *   @param[in]      VectorXd                    Reaction wheel angular momentums [Nm-sec] 
     *   @param[out]     VectorXd                    Power consumption reaction wheels [W]
    */ 

    const VectorXd computeReactionWheelPower( const VectorXd reactionWheelMotorTorque, 
                                              const VectorXd reactionWheelAngularMomentum  ) const 
    {
        VectorXd reactionWheelPowerConsumption( reactionWheelAngularMomentum.size() );
        for ( unsigned int iterator = 0; iterator < reactionWheelAngularMomentum.size(); ++iterator )
        {
            reactionWheelPowerConsumption[iterator] = abs( 1000.0 * reactionWheelMotorTorque[iterator] + 
                                                        4.51 * pow( abs( reactionWheelAngularMomentum[iterator] ), 0.47 ) ); 
        } 
        return reactionWheelPowerConsumption; 
    }
    
protected: 

private: 

const std::vector< ReactionWheel > reactionWheel; 

}; // class 

} // namespace 

#endif 

