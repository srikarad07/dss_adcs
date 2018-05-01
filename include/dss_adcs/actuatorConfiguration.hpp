/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef ACTUATOR_CONFIGURATION_HPP
#define ACTUATOR_CONFIGURATION_HPP

#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{          

class ActuatorConfiguration
{
public: 

    ActuatorConfiguration(    const ReactionWheel& aRw1, 
                              const ReactionWheel& aRw2,
                              const ReactionWheel& aRw3 )
                            : rw1( aRw1 ),
                              rw2( aRw2 ), 
                              rw3( aRw3 )  
    { }

    Vector3 computePrincipleAxesTorque ( ) const 
    {
        Vector3 torque; 
        torque[0]       = rw1.maxTorque; 
        torque[1]       = rw2.maxTorque;
        torque[2]       = rw3.maxTorque;    

        return torque;     
    }
    
protected: 

private: 

const ReactionWheel rw1; 
const ReactionWheel rw2;
const ReactionWheel rw3;

}; // class 

} // namespace 

#endif 