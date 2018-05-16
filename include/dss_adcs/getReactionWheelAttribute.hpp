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

#include <rapidjson/document.h>

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

	std::vector<std::string> keysToRetrieve = {"maximumValue", "value"};
	std::map<std::string, std::string> mapForResult = mapForAttributeThatMatchesName(attributes, "name", "mass", keysToRetrieve);	
	
	for (auto &mapItem : mapForResult) 
	{
    	std::cout << mapItem.first << ":" << mapItem.second << "\n";
	}

	// ReactionWheel reactionWheel( 0.0, 0.0, 0.0, 0.0, 0.1 ); 
	// return reactionWheel; 
} 

} // namespace dss_adcs

#endif //DSS_ADCS_GET_REACTION_WHEEL_ATTRIBUTE_HPP