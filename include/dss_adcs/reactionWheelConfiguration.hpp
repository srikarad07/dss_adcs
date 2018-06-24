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
std::map< std::string, std::vector< ReactionWheel > > getReactionWheelConcepts( const std::string                   actuatorConfigurationType, 
                                                                                const std::vector < ReactionWheel > reactionWheels, 
                                                                                unsigned int                        numberOfReactionWheels )
    {
        std::map< std::string, std::vector< ReactionWheel > > reactionWheelConcepts; 
                         
        if ( actuatorConfigurationType.compare("homogenous") == 0 )
        {
            for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheels.size(); ++ reactionWheelIterator )
            {
                // Save each concept of Reaction wheel to it's respective concept number. 
                std::ostringstream tempReactionWheelConceptNumber;
                tempReactionWheelConceptNumber << reactionWheelIterator + 1; 

                for ( unsigned int iterator = 0; iterator < numberOfReactionWheels; ++iterator )
                {
                    reactionWheelConcepts["concept" + tempReactionWheelConceptNumber.str()].push_back( reactionWheels[reactionWheelIterator] );   
                }    
            }
        
        }
        else if ( actuatorConfigurationType.compare("heterogeneous") == 0 )
        {
            std::cout << actuatorConfigurationType << "Configuration is not defined in the model yet! " << std::endl; 
            throw; 
        }
        else 
        {
            std::cout << actuatorConfigurationType << "Configuration is not defined in the model yet! " << std::endl; 
            throw; 
        }

        return reactionWheelConcepts; 
    }

} // namespace 

#endif 