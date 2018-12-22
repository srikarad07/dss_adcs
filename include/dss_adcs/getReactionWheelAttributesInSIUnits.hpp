/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTES_IN_SI_UNITS_HPP
#define DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTES_IN_SI_UNITS_HPP

#include "math.h"

#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs 
{
    inline double convertToKilograms( 	std::string mass_measurement_unit, 
								std::string mass_value, 
								std::string reactionWheelName, 
								std::string supplierName )
    {
        // Attributes defined as strings in the json file. To convert them into double use std::stod function. 
	    std::string::size_type sz; // size type for stod function. 
    
		double reactionWheelMass( 0.0 );  
	    // Check the measurement unit for the mass. It needs to be kg. 
		if (mass_measurement_unit.compare("kg") != 0 && !mass_value.empty() )
		{
			if (mass_measurement_unit.compare("g") == 0 )
			{
				reactionWheelMass = sml::convertGramsToKilograms( std::stod( mass_value, &sz ) ); 
			}
			else if (mass_measurement_unit.compare("mg") == 0 )
			{
				reactionWheelMass = sml::convertMilligramsToKilograms( std::stod( mass_value, &sz ) ); 
			}
			else 
			{
				std::cout << "The measurement unit " << mass_measurement_unit << "cannot be converted into kgs. The conversion function hasn't been implemented." << std::endl; 
			}
		}
		else if ( mass_value.empty() )
		{
			std::cout << "The reaction wheel " << reactionWheelName << " from " << supplierName << " doesn't have the mass value given. " << std::endl;   
			reactionWheelMass = std::nan("mass"); 
		}
		else 
		{
			reactionWheelMass = std::stod( mass_value, &sz );
		}
		
		return reactionWheelMass; 
	};

	inline double convertToMeter( std::string dimension_measurement_unit,  
						   std::string dimension_value, 
						   std::string reactionWheelName, 
						   std::string supplierName )
	{
		double dimension( 0.0 ); 
		std::string::size_type sz;
	    // Check the measurement unit for the length, height and width. It needs to be 'm'. 
	    if ( dimension_measurement_unit.compare("m") != 0 && !dimension_value.empty() )
	    {
	    	if ( dimension_measurement_unit.compare("cm") == 0 )
	    	{
	    		dimension = sml::convertCentimeterToMeter( std::stod( dimension_value, &sz ) ); 
				// std::cout << "Enters this loop" << std::endl; 
	    	}
	    	else if ( dimension_measurement_unit.compare("mm") == 0 )
	    	{
	    		dimension = sml::convertMillimeterToMeter( std::stod( dimension_value, &sz ) ); 
				// std::cout << "Enters this loop" << std::endl; 
	    	}
	    	else 
	    	{
	    		std::cout << "The measurement unit " << dimension_measurement_unit << "cannot be converted into kgs. The conversion function hasn't been implemented." << std::endl; 
	    	}
	    }
		else if ( dimension_measurement_unit.empty() )
		{
			// std::cout << "The reaction wheel " << reactionWheelName << " from " << supplierName << " doesn't have the dimension value given. " << std::endl;   
			dimension = std::nan("dimension"); 
		}
	    else 
	    {
	    	dimension 	= std::stod( dimension_value, &sz );
	    }
		
		return dimension; 
 	}

	inline double convertToNewtonMeter( std::string torque_measurement_unit,  
						   		 std::string torque_value, 
						   		 std::string reactionWheelName, 
						   		 std::string supplierName )
	{
		double reactionWheelTorque(0.0); 
		std::string::size_type sz;

		if ( torque_measurement_unit.compare("N m") != 0 && !torque_value.empty() )
	    {
	    	reactionWheelTorque		= sml::convertMilliNewtonMeterToNewtonMeter( std::stod( torque_value, &sz ) );
	    }
		else if( torque_value.empty() )
		{  
			reactionWheelTorque = std::nan("Torque"); 
		}
	    else 
	    {
	    	reactionWheelTorque		= std::stod( torque_value, &sz );	
	    }

		return reactionWheelTorque; 
	};

	inline double convertToNewtonMeterSec( 	std::string momentum_storage_measurement_unit,  
						   		 			std::string momentum_storage_value, 
						   		 			std::string reactionWheelName, 
						   		 			std::string supplierName )
	{
		double reactionWheelMomentumStorage(0.0); 
		std::string::size_type sz;

		if ( momentum_storage_measurement_unit.compare("N m s") != 0 && !momentum_storage_value.empty() )
	    {
	    	reactionWheelMomentumStorage		= sml::convertMilliNewtonMeterToNewtonMeter( std::stod( momentum_storage_value, &sz ) );
	    }
		else if( momentum_storage_value.empty() )
		{  
			reactionWheelMomentumStorage = std::nan("Torque"); 
		}
	    else 
	    {
	    	reactionWheelMomentumStorage		= std::stod( momentum_storage_value, &sz );	
	    }

		return reactionWheelMomentumStorage; 
	};

	inline ReactionWheel getReactionWheelAttributesInSiUnits( 
							std::map< std::string, std::pair<std::string, std::string> > mapForResult, 
							std::string 						 reactionWheelName, 
							std::string 						 supplierName, 
							std::string 						 actuatorUuid )
	{	
		const Real reactionWheelMass = convertToKilograms( mapForResult["mass"].second,
												       mapForResult["mass"].first,
												       reactionWheelName,
												       supplierName );
		
		const Real reactionWheelLength = convertToMeter( mapForResult["length"].second,
											         mapForResult["length"].first,
											         reactionWheelName,
											         supplierName );

		const Real reactionWheelHeight = convertToMeter( mapForResult["height"].second,
											         mapForResult["height"].first,
											         reactionWheelName,
											         supplierName );

		const Real reactionWheelWidth = convertToMeter( mapForResult["width"].second,
											        mapForResult["width"].first,
											        reactionWheelName,
											        supplierName );
		
		double reactionWheelTorque; 
		
		//! Check if the value of reaction wheel maximum torque is given and if not 
		//! use the torque value that is given for the reaction wheel. 
		//! <<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
		//  Substituting the value of reaction wheel torque for maximum value where 
		//  it is not given is dangerous. Substitute it with a formula which can be 
		//  used to derive maximum torque. 
		//! <<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
		std::string tempStringValue 	= mapForResult.find("maximum torque")->second.first; 
		if ( tempStringValue.compare("") )
		{
			reactionWheelTorque = convertToNewtonMeter( mapForResult["maximum torque"].second,
											         	mapForResult["maximum torque"].first,
											         	reactionWheelName,
											            supplierName); 
			reactionWheelTorque = 0.7 * reactionWheelTorque; 
		}
		else 
		{
			reactionWheelTorque = convertToNewtonMeter( mapForResult["torque"].second,
											         	mapForResult["torque"].first,
											         	reactionWheelName,
											            supplierName); 
		}

		double reactionWheelMomentumStorage; 
		//! <<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
		//  Substituting the value of reaction wheel torque for maximum value where 
		//  it is not given is dangerous. Substitute it with a formula which can be 
		//  used to derive maximum torque. 
		//! <<<<<<<<<<<<<<<<<<<<<<<<<<<<< TO DO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
		std::string tempStringValue2 	= mapForResult.find("maximum momentum storage")->second.first; 

		if ( tempStringValue2.compare("") )
		{
			reactionWheelMomentumStorage = convertToNewtonMeterSec( 
														mapForResult["maximum momentum storage"].second,
											         	mapForResult["maximum momentum storage"].first,
											         	reactionWheelName,
											            supplierName); 
		}
		else 
		{
			reactionWheelMomentumStorage = convertToNewtonMeterSec( 
														mapForResult["angular momentum storage"].second,
											         	mapForResult["angular momentum storage"].first,
											         	reactionWheelName,
											            supplierName); 
		}

		// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TEMP >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> // 
		// Make it into a proper function like it has been done above. 
		double reactionWheelPeakPower(0.0); 
		std::string::size_type sz;
		if (mapForResult["maximum power"].second.compare("mW") != 0 && !mapForResult["maximum power"].first.empty())
		{
			reactionWheelPeakPower 		= std::stod( mapForResult["maximum power"].first, &sz ); 
		}
		else if ( mapForResult["maximum power"].second.compare("W") != 0 && !mapForResult["maximum power"].first.empty() )
		{
			reactionWheelPeakPower 		= std::stod( mapForResult["maximum power"].first, &sz ) / 1000.0; 
		}
		// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<       >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //  
		const Real reactionWheelRadius = ( 1.0/2.0 ) * convertToMeter( mapForResult["diameter"].second,
											         	   mapForResult["diameter"].first,
											         	   reactionWheelName,
											               supplierName) ; 

		const ReactionWheel reactionWheel( reactionWheelMass, reactionWheelLength, reactionWheelHeight, reactionWheelWidth,reactionWheelRadius, reactionWheelTorque, reactionWheelMomentumStorage, reactionWheelPeakPower, reactionWheelName, supplierName, actuatorUuid ); 
		
		return reactionWheel; 
	} 

}      
    
#endif // DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTES_IN_SI_UNITS_HPP