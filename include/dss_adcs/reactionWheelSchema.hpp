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
public: 

    Vector2 wheelOrientation; 

    ReactionWheel(  const Real          aMass, 
                    const Real          aLength, 
                    const Real          aHeight, 
                    const Real          aWidth, 
                    const Real          aRadius, 
                    const Real          aMaxTorque, 
                    const Real          aMaxMomentumStorage,
                    const Real          aPeakPower,
                    const std::string   aName, 
                    const std::string   aSupplierName   )
                :   mass( aMass ),
                    length( aLength ),
                    height( aHeight ),
                    width( aWidth ),
                    radius( aRadius ), 
                    maxTorque( aMaxTorque ), 
                    maxMomentumStorage( aMaxMomentumStorage ), 
                    peakPower( aPeakPower ), 
                    name( aName ), 
                    supplierName( aSupplierName )
    { }

    const Real mass; 

    const Real length; 

    const Real height; 

    const Real width;

    const Real radius; 

    const Real maxTorque;
    
    const Real maxMomentumStorage; 

    const Real peakPower;
    
    const std::string name; 

    const std::string supplierName;

protected: 

private: 

}; 

}
 #endif 