/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef REACTION_WHEEL_SCHEMA_HPP
#define REACTION_WHEEL_SCHEMA_HPP

#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{

struct ReactionWheel
{
    Real mass; 
    Real length; 
    Real height; 
    Real width; 
    Real maxTorque; 
}; 

}
 #endif 