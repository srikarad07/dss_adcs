/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/
    
#ifndef GET_REACTION_WHEEL_HPP
#define GET_REACTION_WHEEL_HPP

#include "dss_adcs/apiCall.hpp"
// #include "dss_adcs/getReactionWheelAttribute.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{
   
    std::vector < ReactionWheel > getReactionWheels(   const std::string actuator, 
                                                        const std::vector< std::string >actuatorUuid )
    {
        std::vector < ReactionWheel > reactionWheels;
        
        // Get reaction wheels from the API.
        // for ( unsigned int iterator = 0; iterator < actuator.size(); ++iterator )
        // {
        //     std::string reactionWheelString = callTheApi( actuatorUuid[iterator] );
        //     ReactionWheel reactionWheel     = getReactionWheelAttributes( reactionWheelString ); 
        //     reactionWheels.push_back( reactionWheel ); 
        // }

        const ReactionWheel reactionWheel1( 0.0, 0.0, 0.0, 0.0, 0.1 ), reactionWheel2( 0.0, 0.0, 0.0, 0.0, 0.1 ), reactionWheel3( 0.0, 0.0, 0.0, 0.0, 0.1 ); 
         
        reactionWheels.push_back( reactionWheel1 );
        reactionWheels.push_back( reactionWheel2 );
        reactionWheels.push_back( reactionWheel3 );

        return reactionWheels;
    }
    
}

#endif //GET_REACTION_WHeel_HPP