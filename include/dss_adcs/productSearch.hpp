/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_PRODUCT_SEARCH_HPP
#define DSS_ADCS_PRODUCT_SEARCH_HPP

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
    //! Class containing members and functions that are required to search for a product 
    //! and extract it's attributes. 
    /*  This class contains the members and function required to search for the required
    *   product and extract the attributes for that particular product. 
    */ 

class ProductSearch
{
public: 
    // ProductSearch( const std::string aProductFolderPath )
    //         : productFolderPath( aProductFolderPath )   
     ProductSearch( )   
    { }

void operator( )( )
{
    std::ifstream inputFile( "/home/superman/trial/curl/product.json" );
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

	int counter = 0; 
 
	for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) 
	{
    	const rapidjson::Value& attribute = *itr;
    	assert(attribute.IsObject()); // each attribute is an object
    	for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) 
		{	
			// std::string value = itr2->value.GetString();
			if ( !itr2->value.IsNull() )
			{
				std::cout << itr2->name.GetString() << " : " << "No value defined" << "\n" << std::endl;	
			}
			// else 
			// {
			// 	std::cout << itr2->name.GetString() << " : " << itr2->value.GetString() << "\n" << std::endl; 
			// }
		}
	} 
} // void 

private: 
// const std::string productFolderPath; 

}; // class ProductSearch 

} // namespace dss_adcs

#endif //DSS_ADCS_PRODUCT_SEARCH_HPP