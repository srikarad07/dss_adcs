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

inline ReactionWheel getReactionWheelAttributes( const std::string 	productJson )
{	
	rapidjson::Document config;

	config.Parse<0>(productJson.c_str()).HasParseError();

	assert(config.IsObject()); 
	
	const std::string reactionWheelName 	= config["name"].GetString();
	const std::string supplierName 			= config["supplier_name"].GetString();
	
	// std::cout << "Reaction wheel name: 	" << reactionWheelName << std::endl; 
	// std::cout << "Supplier name: 		" << supplierName << std::endl; 
	
	const rapidjson::Value& attributes 	= config["attributes"];
	assert(attributes.IsArray()); 

	std::vector<std::string> keysToRetrieve = {"measurement_unit", "value"};
	std::vector< std::string > attributesToRetrieve = { "mass", "length", "width", "height", "maximum torque", "torque", "diameter", "angular momentum storage", "maximum momentum storage", "maximum power" };
	std::map<std::string, std::string> mapForResult = mapForAttributeThatMatchesName( attributes, "name", attributesToRetrieve, keysToRetrieve );	

	const ReactionWheel reactionWheel = getReactionWheelAttributesInSiUnits( mapForResult, 
																			 reactionWheelName,
												       						 supplierName ); 

	return reactionWheel; 
} 

} // namespace dss_adcs

#endif //DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTE_HPP