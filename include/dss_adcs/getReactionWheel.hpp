/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/
    
#ifndef DSS_ADCS_GET_REACTION_WHEEL_HPP
#define DSS_ADCS_GET_REACTION_WHEEL_HPP

#include <math.h> 

#include "dss_adcs/apiCall.hpp"
#include "dss_adcs/getReactionWheelAttribute.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{
   
    inline std::vector < ReactionWheel > getReactionWheels( const std::string actuator, 
                                                            const std::vector< std::string >actuatorUuid )
    {
        std::vector < ReactionWheel > reactionWheels;

        // Get reaction wheels from the API.
        for ( unsigned int iterator = 0; iterator < actuatorUuid.size(); ++iterator )
        {
            std::string reactionWheelString = callTheApi( actuatorUuid[iterator] );
            ReactionWheel tempReactionWheel = getReactionWheelAttributes( reactionWheelString ); 

            if ( !isnan( tempReactionWheel.maxTorque ) )
            {
                reactionWheels.push_back( tempReactionWheel ); 
            }
            else 
            {
                std::cout << "The reaction wheel " << tempReactionWheel.name << " from " << tempReactionWheel.supplierName << "doesn't have the torque value given and hence, will not be considered for the simulations! " << std::endl;  
            }
        }
        
        return reactionWheels;
    }
    
}

#endif //GET_REACTION_WHeel_HPP