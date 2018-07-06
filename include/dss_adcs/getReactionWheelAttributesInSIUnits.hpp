/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTES_IN_SI_UNITS_HPP
#define DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTES_IN_SI_UNITS_HPP

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
	    	if ( dimension_measurement_unit.compare("cm") != 0 )
	    	{
	    		dimension = sml::convertCentimeterToMeter( std::stod( dimension_value, &sz ) ); 
	    	}
	    	else if ( dimension_measurement_unit.compare("mm") != 0 )
	    	{
	    		dimension = sml::convertMillimeterToMeter( std::stod( dimension_value, &sz ) ); 
	    	}
	    	else 
	    	{
	    		std::cout << "The measurement unit " << dimension_measurement_unit << "cannot be converted into kgs. The conversion function hasn't been implemented." << std::endl; 
	    	}
	    }
		else if ( dimension_measurement_unit.empty() )
		{
			std::cout << "The reaction wheel " << reactionWheelName << " from " << supplierName << " doesn't have the dimension value given. " << std::endl;   
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

	inline ReactionWheel getReactionWheelAttributesInSiUnits( std::map< std::string, std::string > mapForResult, 
													   		  std::string 							reactionWheelName, 
													   		  std::string 							supplierName )
	{	
		const Real reactionWheelMass = convertToKilograms( mapForResult["mass-measurement_unit"],
												       mapForResult["mass-value"],
												       reactionWheelName,
												       supplierName );
		
		const Real reactionWheelLength = convertToMeter( mapForResult["length-measurement_unit"],
											         mapForResult["length-value"],
											         reactionWheelName,
											         supplierName );

		const Real reactionWheelHeight = convertToMeter( mapForResult["height-measurement_unit"],
											         mapForResult["height-value"],
											         reactionWheelName,
											         supplierName );

		const Real reactionWheelWidth = convertToMeter( mapForResult["width-measurement_unit"],
											        mapForResult["width-value"],
											        reactionWheelName,
											        supplierName );
		
		double reactionWheelTorque; 

		if ( mapForResult.find("maximum torque-measurement_unit") != mapForResult.end() )
		{
			reactionWheelTorque = convertToNewtonMeter( mapForResult["maximum torque-measurement_unit"],
											         	   mapForResult["maximum torque-value"],
											         	   reactionWheelName,
											               supplierName); 
		}
		else 
		{
			reactionWheelTorque = convertToNewtonMeter( mapForResult["torque-measurement_unit"],
											         	   mapForResult["torque-value"],
											         	   reactionWheelName,
											               supplierName); 
		}

		const ReactionWheel reactionWheel( reactionWheelMass, reactionWheelLength, reactionWheelWidth, reactionWheelHeight, reactionWheelTorque, reactionWheelName, supplierName ); 
		
		return reactionWheel; 
	} 

}      
    
#endif // DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTES_IN_SI_UNITS_HPP