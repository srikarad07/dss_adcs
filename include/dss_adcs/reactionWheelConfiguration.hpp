/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_REACTION_WHEEL_CONFIGURATION_HPP
#define DSS_ADCS_REACTION_WHEEL_CONFIGURATION_HPP

#include <iostream>
#include <stdlib.h> 
#include <string>

#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{
std::map< std::string, std::vector< ReactionWheel > > getReactionWheelConcepts( const std::string                   actuatorConfigurationType, 
                                                                                const std::vector < ReactionWheel > reactionWheels, 
                                                                                unsigned int                        numberOfReactionWheels, 
                                                                                const std::vector < Vector2 >       wheelOrientation  )
    {
        std::map< std::string, std::vector< ReactionWheel > > reactionWheelConcepts; 
                         
        if ( actuatorConfigurationType.compare("homogenous") == 0 )
        {
            for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheels.size(); ++reactionWheelIterator )
            {
                // Save each concept of Reaction wheel to it's respective concept number. 
                std::ostringstream tempReactionWheelConceptNumber;
                tempReactionWheelConceptNumber << reactionWheelIterator + 1; 
                for ( unsigned int iterator = 0; iterator < numberOfReactionWheels; ++iterator )
                {
                    ReactionWheel tempReactionWheel     = reactionWheels[reactionWheelIterator];
                    tempReactionWheel.wheelOrientation[0]  = wheelOrientation[iterator][0]; 
                    tempReactionWheel.wheelOrientation[1]  = wheelOrientation[iterator][1]; 

                    reactionWheelConcepts["concept_" + tempReactionWheelConceptNumber.str()].push_back( tempReactionWheel );   
                }    
            }
        
        }
        else if ( actuatorConfigurationType.compare("random") == 0 )
        {
            for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < 3*reactionWheels.size(); ++ reactionWheelIterator )
            {
                // Save each concept of Reaction wheel to it's respective concept number. 
                std::ostringstream tempReactionWheelConceptNumber;
                tempReactionWheelConceptNumber << reactionWheelIterator + 1; 
                // <<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
                // Check if this is the right way to put the reaction wheels together // 
                // <<<<<<<<<<<<<<<<<<<<<<<        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
                for ( unsigned int iterator = 0; iterator < numberOfReactionWheels; ++iterator )
                {
                    ReactionWheel tempReactionWheel     = reactionWheels[reactionWheelIterator];
                    tempReactionWheel.wheelOrientation[0]  = wheelOrientation[iterator][0]; 
                    tempReactionWheel.wheelOrientation[1]  = wheelOrientation[iterator][1]; 
                    unsigned int reactionWheelNumber = rand () % reactionWheels.size(); 
                    reactionWheelConcepts["concept_" + tempReactionWheelConceptNumber.str()].push_back( reactionWheels[reactionWheelNumber] );   
                }    
            }
        }
        else if ( actuatorConfigurationType.compare("heterogenous") == 0 )
        {
            std::cout << actuatorConfigurationType << "Configuration is not defined in the model yet! " << std::endl; 
            throw; 
            // for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < 3*reactionWheels.size(); ++ reactionWheelIterator )
            // {
            //     // Save each concept of Reaction wheel to it's respective concept number. 
            //     std::ostringstream tempReactionWheelConceptNumber;
            //     tempReactionWheelConceptNumber << reactionWheelIterator + 1; 

            //     for ( unsigned int iterator = 0; iterator < numberOfReactionWheels; ++iterator )
            //     {
            //         unsigned int reactionWheelNumber = numberOfReactionWheels; 
            //         reactionWheelConcepts["concept" + tempReactionWheelConceptNumber.str()].push_back( reactionWheels[reactionWheelNumber] );   
            //     }    
            // }
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