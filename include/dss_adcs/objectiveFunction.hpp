/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_OBJECTIVE_FUNCTION_HPP
#define DSS_ADCS_OBJECTIVE_FUNCTION_HPP

#include <algorithm>

#include "dss_adcs/typedefs.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"

namespace dss_adcs
{

class ObjectiveFunction 
{

public: 
    
    Real massBudget; 
    Real volumeBudget; 
    Real accuracy; 
    Real tumblingRate;
    Real saturationRate; 

    ObjectiveFunction(  const std::vector< ReactionWheel >  aReactionWheelConcepts,
                        const std::vector< Vector4 >        aQuaternionStateVector, 
                        const std::string                   aStateHistoryFilePath )
                : reactionWheelConcept( aReactionWheelConcepts ), 
                  quaternionStateVector( aQuaternionStateVector ),
                  stateHistoryFilePath( aStateHistoryFilePath )
    { }
    
    Real computeObjectiveFunction( )
    {
        // < TO DO: confirm the way volume budget is used. It might not be correct. > // 
        // < CHECK: The overall calculation of the objective function might not be correct. > // 
        // Weights on the system: USER DEFINED weights: Should be obtained from the input file. //
        // Real w1, w2, w3, w4, w5, w6;
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //  
        Real length(0.0), width(0.0), height(0.0); 
        for ( unsigned int iterator = 0; iterator < reactionWheelConcept.size(); ++iterator )
        {
            massBudget  +=  reactionWheelConcept[iterator].mass;
            // std::cout << "Reaction WHeel mass; " << reactionWheelConcept[iterator].mass << std::endl; 
            length      +=  reactionWheelConcept[iterator].length;
            width       +=  reactionWheelConcept[iterator].width;
            height      +=  reactionWheelConcept[iterator].height;  
            // << TO DO: Height and width values mixed up>>
            // std::cout << "Length: " << reactionWheelConcept[iterator].length << std::endl;
            // std::cout << "Width: " << reactionWheelConcept[iterator].width << std::endl;
            // std::cout << "height: " << reactionWheelConcept[iterator].height << std::endl; 
            // if(std::find( quaternionStateVector[0].begin(), quaternionStateVector[0].end(), minimumAttitudeError[0] ) != quaternionStateVector[0].end() )
            // {
            //     std::cout << "IT works! " << std::endl; 
            // }
        }

        // for( unsigned int iterator2 = 0; iterator2 < quaternionStateVector.size(); ++iterator2 )
        // {

        // }
        volumeBudget     = length * height * width;  
        std::cout << "Mass budget: " << volumeBudget << std::endl; 
        Real finalObjective  = volumeBudget + massBudget;
         
        return finalObjective;  
    }        

private: 

    //! Reaction wheel concepts 
    const std::vector< ReactionWheel > reactionWheelConcept; 

    //! Quaternion state vector values for all times. 
    const std::vector< Vector4 > quaternionStateVector; 

    //! File path for the state history. 
    const std::string stateHistoryFilePath; 

protected: 

}; 

} // namespace dss_adcs

#endif 