/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTE_HPP
#define DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTE_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept> 
#include <locale> 

#include <typeinfo>

#include <rapidjson/document.h>

#include <sml/sml.hpp>

#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/tools.hpp"

namespace dss_adcs
{

ReactionWheel getReactionWheelAttributes( const std::string productJson )
// void getReactionWheelAttributes(  )
{
    // std::ifstream inputFile( productJson );

	// std::stringstream jsonDocumentBuffer;
	// std::string inputLine;

	// while ( std::getline( productJson, inputLine ) )
	// {
	//      jsonDocumentBuffer << inputLine << "\n";
	// }
	rapidjson::Document config;
	// config.Parse( jsonDocumentBuffer.str( ).c_str( ) );
	config.Parse<0>(productJson.c_str()).HasParseError();

	assert(config.IsObject()); 
	
	const rapidjson::Value& attributes 	= config["attributes"];
	assert(attributes.IsArray()); 

	std::vector<std::string> keysToRetrieve = {"measurement_unit", "value"};
	std::vector< std::string > attributesToRetrieve = { "mass", "length", "width", "height", "maximum torque" };
	std::map<std::string, std::string> mapForResult = mapForAttributeThatMatchesName( attributes, "name", attributesToRetrieve, keysToRetrieve);	

	Real reactionWheelMass(0.0);
	Real reactionWheellength(0.0);
	Real reactionWheelWidth(0.0);
	Real reactionWheelHeight(0.0);
	Real reactionWheelTorque(0.0); 

	// Attributes defined as strings in the json file. To convert them into double use std::stod function. 
	std::string::size_type sz; // size type for stod function. 

	// Check the measurement unit for the mass. It needs to be kg. 
	if ( mapForResult["mass-measurement_unit"].compare("kg") != 0 )
	{
		if ( mapForResult["mass-measurement_unit"].compare("g") == 0 )
		{
			reactionWheelMass = sml::convertGramsToKilograms( std::stod( mapForResult["mass-value"], &sz ) ); 
		}
		else if ( mapForResult["mass-measurement_unit"].compare("mg") == 0 )
		{
			reactionWheelMass = sml::convertMilligramsToKilograms( std::stod( mapForResult["mass-value"], &sz ) ); 
		}
		else 
		{
			std::cout << "The measurement unit " << mapForResult[ "mass-measurement_unit"] << "cannot be converted into kgs. The conversion function hasn't been implemented." << std::endl; 
		}
	}
	else 
	{
		reactionWheelMass = std::stod( mapForResult["mass-value"], &sz );
	}
	// std::cout << "It went okay until mass extration! " << std::endl; 
	// Check the measurement unit for the length, height and width. It needs to be 'm'. 
	if ( mapForResult["length-measurement_unit"].compare("m") != 0 )
	{
		if ( mapForResult["length-measurement_unit"].compare("cm") != 0 )
		{
			reactionWheellength = sml::convertCentimeterToMeter( std::stod( mapForResult["length-value"], &sz ) ); 
			reactionWheelWidth = sml::convertGramsToKilograms( std::stod( mapForResult["width-value"], &sz ) ); 
			reactionWheelHeight = sml::convertGramsToKilograms( std::stod( mapForResult["height-value"], &sz ) ); 
		}
		else if ( mapForResult["length-measurement_unit"].compare("mm") != 0 )
		{
			reactionWheellength = sml::convertMillimeterToMeter( std::stod( mapForResult["length-value"], &sz ) ); 
			reactionWheelWidth = sml::convertMilligramsToKilograms( std::stod( mapForResult["width-value"], &sz ) ); 
			reactionWheelHeight = sml::convertMilligramsToKilograms( std::stod( mapForResult["height-value"], &sz ) ); 
		}
		else 
		{
			std::cout << "The measurement unit " << mapForResult[ "length-measurement_unit"] << "cannot be converted into kgs. The conversion function hasn't been implemented." << std::endl; 
		}
	}
	else 
	{
		reactionWheellength 	= std::stod( mapForResult["length-value"], &sz );
		reactionWheelWidth 		= std::stod( mapForResult["width-value"], &sz );
		reactionWheelHeight 	= std::stod( mapForResult["height-value"], &sz );
	}
	// std::cout << "It went okay until length-width-height extration! " << std::endl; 
	// std::cout << "Measurement unit " << mapForResult["maximum torque-measurement_unit"] << std::endl; 
	if ( mapForResult["maximum torque-measurement_unit"].compare("N m") != 0 )
	{
		reactionWheelTorque		= sml::convertMilliNewtonMeterToNewtonMeter( std::stod( mapForResult["maximum torque-value"], &sz ) );
	}
	else 
	{
		reactionWheelTorque		= std::stod( mapForResult["maximum torque-value"], &sz );	
	}
	std::cout << "Torque: " << reactionWheelTorque << std::endl;
	std::cout << "Mass: " << reactionWheelMass << std::endl;	 
	
	// ReactionWheel reactionWheel; 
	ReactionWheel reactionWheel( reactionWheelMass, reactionWheellength, reactionWheelWidth, reactionWheelHeight, reactionWheelTorque ); 
	return reactionWheel; 
} 

} // namespace dss_adcs

#endif //DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTE_HPP