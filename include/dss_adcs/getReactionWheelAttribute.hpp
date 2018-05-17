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

// ReactionWheel getReactionWheelAttributes(  )
void getReactionWheelAttributes(  )
{
    std::ifstream inputFile( "/home/superman/trial/curl/rw1.json" );

	std::stringstream jsonDocumentBuffer;
	std::string inputLine;

	while ( std::getline( inputFile, inputLine ) )
	{
	     jsonDocumentBuffer << inputLine << "\n";
	}
	rapidjson::Document config;
	config.Parse( jsonDocumentBuffer.str( ).c_str( ) );

	assert(config.IsObject()); 
	
	const rapidjson::Value& attributes 	= config["attributes"];
	assert(attributes.IsArray()); 

	std::vector<std::string> keysToRetrieve = {"measurementUnit", "value"};
	std::vector< std::string > attributesToRetrieve = { "mass", "length", "width", "height", "maximum-torque" };
	std::map<std::string, std::string> mapForResult = mapForAttributeThatMatchesName( attributes, "name", "mass", keysToRetrieve);	

	Real reactionWheelMass; 
	// Attributes defined as strings in the json file. To convert them into double use std::stod function. 
	std::string::size_type sz; // size type for stod function. 
 
	if ( mapForResult["mass-measurementUnit"].compare("kg") != 0 )
	{
		if ( mapForResult["mass-measurementUnit"].compare("g") != 0 )
		{
			reactionWheelMass = sml::convertGramsToKilograms( std::stod( mapForResult["mass-value"], &sz ) ); 
		}
		else if ( mapForResult["mass-measurementUnit"].compare("mg") != 0 )
		{
			reactionWheelMass = sml::convertMilligramsToKilograms( std::stod( mapForResult["mass-value"], &sz ) ); 
		}
		else 
		{
			std::cout << "The measurement unit " << mapForResult[ "mass-measurementUnit"] << "cannot be converted into kgs. The conversion function hasn't been implemented." << std::endl; 
		}
	}
	else 
	{
		reactionWheelMass = std::stod( mapForResult["mass-value"], &sz );
	}

	// ReactionWheel reactionWheel(  0.0, 0.0, 0.0, 0.0, 0.1 ); 
	// return reactionWheel; 
} 

} // namespace dss_adcs

#endif //DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTE_HPP