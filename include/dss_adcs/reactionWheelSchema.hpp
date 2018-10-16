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
                    const std::string   aSupplierName, 
                    const std::string   anActuatorUuid   )
                :   mass( aMass ),
                    length( aLength ),
                    height( aHeight ),
                    width( aWidth ),
                    radius( aRadius ), 
                    maxTorque( aMaxTorque ), 
                    maxMomentumStorage( aMaxMomentumStorage ), 
                    peakPower( aPeakPower ), 
                    name( aName ), 
                    supplierName( aSupplierName ), 
                    actuatorUuid( anActuatorUuid )
    { }

    //! Save the mass attribute.
    const Real mass; 

    //! Save the mass length.
    const Real length; 

    //! Save the mass height.
    const Real height; 

    //! Save the mass width.
    const Real width;

    //! Save the radius attribute.
    const Real radius; 

    //! Save the maximum torque attribute.
    const Real maxTorque;
    
    //! Save the max momentum storage attribute.
    const Real maxMomentumStorage; 

    //! Save the peak power attribute.
    const Real peakPower;

    //! Save the name of the reaction wheel attribute.    
    const std::string name; 

    //! Save the supplier name attribute.
    const std::string supplierName;

    //! Save the actuatorUuid attribute.
    const std::string actuatorUuid; 

protected: 

private: 

}; 

}
 #endif 