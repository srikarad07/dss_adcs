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
//! Function to generate reaction wheel concepts for the given settings. 
/* This function generates reaction wheel concepts based on the settings defined by the user 
 * such as 'homogenous' (same reaction wheel )or 'random' pair (different reaction wheels combine to form a concept)    
 * The output is a std::map which consists of a std::pair with concept identifier and number of reaction wheels 
 * and a std::vector of reaction wheel structure. 
*/

std::map< std::pair<std::string, unsigned int>, std::vector< ReactionWheel > > getReactionWheelConcepts( const std::string                   actuatorConfigurationType, 
                                                                                const std::vector < ReactionWheel > reactionWheels, 
                                                                                unsigned int                        minimumReactionWheelNumber, 
                                                                                unsigned int                        maximumReactionWheelNumber,
                                                                                const std::vector < Vector2 >       wheelOrientation  )
    {
          
        std::map< std::pair<std::string, unsigned int>, std::vector< ReactionWheel > > reactionWheelConcepts; 
                         
        if ( actuatorConfigurationType.compare("homogenous") == 0 )
        {
            unsigned int numberOfReactionWheels(0);
            for( unsigned int numberOfReactionWheelsIterator = minimumReactionWheelNumber; numberOfReactionWheelsIterator < maximumReactionWheelNumber;  ++numberOfReactionWheelsIterator )
            {
                numberOfReactionWheels = numberOfReactionWheelsIterator; 
                for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < reactionWheels.size(); ++reactionWheelIterator )
                {
                    // Save each concept of Reaction wheel to it's respective concept number. 
                    std::ostringstream tempReactionWheelConceptNumber;
                    tempReactionWheelConceptNumber << reactionWheelIterator + 1; 
                    for ( unsigned int iterator = 0; iterator < numberOfReactionWheels; ++iterator )
                    {
                        ReactionWheel tempReactionWheel         = reactionWheels[reactionWheelIterator];
                        tempReactionWheel.wheelOrientation[0]   = wheelOrientation[iterator][0]; 
                        tempReactionWheel.wheelOrientation[1]   = wheelOrientation[iterator][1]; 
                        std::pair< std::string, unsigned int> conceptIdentifier("concept_" + tempReactionWheelConceptNumber.str(), numberOfReactionWheels); 
                        reactionWheelConcepts[conceptIdentifier].push_back( tempReactionWheel );   
                    }    
                }
            }         
        }
        // <<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
        // FIX THE REACTION WHEEL RANDOM GENERATOR FUNCTION            // 
        // <<<<<<<<<<<<<<<<<<<<        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 

        // else if ( actuatorConfigurationType.compare("random") == 0 )
        // {
        //     for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < 3*reactionWheels.size(); ++ reactionWheelIterator )
        //     {
        //         // Save each concept of Reaction wheel to it's respective concept number. 
        //         std::ostringstream tempReactionWheelConceptNumber;
        //         tempReactionWheelConceptNumber << reactionWheelIterator + 1; 
        //         // <<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
        //         // Check if this is the right way to put the reaction wheels together // 
        //         // <<<<<<<<<<<<<<<<<<<<<<<        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
        //         for ( unsigned int iterator = 0; iterator < numberOfReactionWheels; ++iterator )
        //         {
        //             ReactionWheel tempReactionWheel     = reactionWheels[reactionWheelIterator];
        //             tempReactionWheel.wheelOrientation[0]  = wheelOrientation[iterator][0]; 
        //             tempReactionWheel.wheelOrientation[1]  = wheelOrientation[iterator][1]; 
        //             unsigned int reactionWheelNumber = rand () % reactionWheels.size(); 
        //             reactionWheelConcepts["concept_" + tempReactionWheelConceptNumber.str()].push_back( reactionWheels[reactionWheelNumber] );   
        //         }    
        //     }
        // }
        // else if ( actuatorConfigurationType.compare("heterogenous") == 0 )
        // {
        //     std::cout << actuatorConfigurationType << "Configuration is not defined in the model yet! " << std::endl; 
        //     throw; 
        //     // for( unsigned int reactionWheelIterator = 0; reactionWheelIterator < 3*reactionWheels.size(); ++ reactionWheelIterator )
        //     // {
        //     //     // Save each concept of Reaction wheel to it's respective concept number. 
        //     //     std::ostringstream tempReactionWheelConceptNumber;
        //     //     tempReactionWheelConceptNumber << reactionWheelIterator + 1; 

        //     //     for ( unsigned int iterator = 0; iterator < numberOfReactionWheels; ++iterator )
        //     //     {
        //     //         unsigned int reactionWheelNumber = numberOfReactionWheels; 
        //     //         reactionWheelConcepts["concept" + tempReactionWheelConceptNumber.str()].push_back( reactionWheels[reactionWheelNumber] );   
        //     //     }    
        //     // }
        // }
        else 
        {
            std::cout << actuatorConfigurationType << "Configuration is not defined in the model yet! " << std::endl; 
            throw; 
        }

        return reactionWheelConcepts; 
    }

} // namespace 

#endif 