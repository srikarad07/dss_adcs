/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_REACTION_WHEEL_CONFIGURATION_HPP
#define DSS_ADCS_REACTION_WHEEL_CONFIGURATION_HPP

#include <iostream>
#include <string>

#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{
    std::vector< ReactionWheel > getReactionWheelConcept( const std::string                   actuatorConfigurationType, 
                                                          const std::vector< ReactionWheel >  reactionWheels, 
                                                          const int                           reactionWheelNumber, 
                                                          const std::vector< Vector2 >        wheelOrientation )
    {
        std::vector< ReactionWheel > reactionWheelConcept; 
        
        if ( actuatorConfigurationType.compare("homogenous") == 0 )
        {
            for ( unsigned int iterator = 0; iterator < wheelOrientation.size(); ++iterator )
            {
                ReactionWheel reactionWheel     = reactionWheels[reactionWheelNumber]; 
                reactionWheelConcept.push_back( reactionWheel ); 
            }
        }
        else 
        {
            std::cout << actuatorConfigurationType << "configuration is not defined in the model yet! " << std::endl; 
        }

        return reactionWheelConcept; 
    }

} // namespace 

#endif 