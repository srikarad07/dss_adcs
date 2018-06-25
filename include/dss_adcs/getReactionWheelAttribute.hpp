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

#include "dss_adcs/getReactionWheelAttributesInSIUnits.hpp"
#include "dss_adcs/reactionWheelSchema.hpp"
#include "dss_adcs/tools.hpp"

namespace dss_adcs
{

inline ReactionWheel getReactionWheelAttributes( const std::string 	productJson, 
												 const Vector2  	wheelOrientation )
{
    // std::ifstream inputFile( productJson );

	// std::stringstream jsonDocumentBuffer;
	// std::string inputLine;

	// while ( std::getline( productJson, inputLine ) )
	// {
	//      jsonDocumentBuffer << inputLine << "\n";
	// }
	// config.Parse( jsonDocumentBuffer.str( ).c_str( ) );
	
	rapidjson::Document config;

	config.Parse<0>(productJson.c_str()).HasParseError();

	assert(config.IsObject()); 
	
	const std::string reactionWheelName 	= config["name"].GetString();
	const std::string supplierName 			= config["supplier_name"].GetString();
	
	std::cout << "Reaction wheel name: 	" << reactionWheelName << std::endl; 
	std::cout << "Supplier name: 		" << supplierName << std::endl; 
	
	const rapidjson::Value& attributes 	= config["attributes"];
	assert(attributes.IsArray()); 

	std::vector<std::string> keysToRetrieve = {"measurement_unit", "value"};
	std::vector< std::string > attributesToRetrieve = { "mass", "length", "width", "height", "maximum torque" };
	std::map<std::string, std::string> mapForResult = mapForAttributeThatMatchesName( attributes, "name", attributesToRetrieve, keysToRetrieve );	

	const Real reactionWheelMass = convertToKilograms( mapForResult["mass-measurement_unit"],
												       mapForResult["mass-value"],
												       reactionWheelName,
												       supplierName );

	const Real reactionWheellength = convertToMeter( mapForResult["length-measurement_unit"],
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

	const Real reactionWheelTorque = convertToNewtonMeter( mapForResult["maximum torque-measurement_unit"],
											         	   mapForResult["maximum torque-value"],
											         	   reactionWheelName,
											               supplierName); 
	
	// ReactionWheel reactionWheel; 
	ReactionWheel reactionWheel( wheelOrientation, reactionWheelMass, reactionWheellength, reactionWheelWidth, reactionWheelHeight, reactionWheelTorque, reactionWheelName, supplierName ); 

	return reactionWheel; 
} 

} // namespace dss_adcs

#endif //DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTE_HPP