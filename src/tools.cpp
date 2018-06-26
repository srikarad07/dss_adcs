/*
 * Copyright (c) 2017, K. Kumar, Delft University of Technology (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <sstream>
#include <stdexcept>
#include <locale> 
#include <iostream>
#include <string>

#include "dss_adcs/tools.hpp"

namespace dss_adcs
{

//! Find parameter.
ConfigIterator find( const rapidjson::Document& config, const std::string& parameterName )
{
    const ConfigIterator iterator = config.FindMember( parameterName.c_str( ) );
    if ( iterator == config.MemberEnd( ) )
    {
        std::ostringstream error;
        error << "ERROR: \"" << parameterName << "\" missing from config file!";
        throw std::runtime_error( error.str( ) );
    }
    return iterator;
}

//! Check the sign of a double. 
double signFunction( double x )
{
        if ( x > 0 )
        {
            return 1.0; 
        }
        else 
        {
            return -1.0;
        }
} 

//! Extract attributes from the json file. 
std::map<std::string, std::string> mapForAttributeThatMatchesName(const rapidjson::Value& attributes, const std::string& findMemberName, const std::vector< std::string >findMemberValue, const std::vector<std::string>& keysToRetrieve) 
{   
    std::map<std::string, std::string> result;
    for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) 
    {
        const rapidjson::Value& attributeClass = (*itr)["class"]; 
        for (rapidjson::Value::ConstMemberIterator itr2 = attributeClass.MemberBegin(); itr2 != attributeClass.MemberEnd(); ++itr2 )
        {  
            if ( findMemberName == itr2->name.GetString() && std::any_of( findMemberValue.cbegin(), findMemberValue.cend(), [&itr2]( std::string attributeString ){ return attributeString == itr2->value.GetString(); } ) ) 
            {   
                std::string attributeName = itr2->value.GetString(); 
                std::cout << attributeName << std::endl; 
                for (auto &keyToRetrieve : keysToRetrieve) 
                {
                    // std::cout << "Keys! " << keysToRetrieve[0] << std::endl; 
                    const rapidjson::Value::ConstMemberIterator currentAttributeToReturn = itr->FindMember(keyToRetrieve.c_str());
                    // std::cout << currentAttributeToReturn->value.IsNull() << std::endl; 
                    
                    const std::string currentAttributeValue = currentAttributeToReturn->value.GetString();

                    if (currentAttributeToReturn != itr->MemberEnd() && currentAttributeToReturn->value.IsString() && currentAttributeValue.compare("") != 0 ) 
                    {
                        // std::cout << currentAttributeToReturn->value.IsNull() << std::endl; 
                        // std::cout << "Map element test: " << attributeName + "-" + keyToRetrieve << std::endl;
                        result[attributeName + "-" + keyToRetrieve] = currentAttributeToReturn->value.GetString();
                        // std::cout << currentAttributeToReturn->value.GetString() << std::endl; 
                    }
                    // else if ( currentAttributeValue.compare("") == 0 && )
                    // {
                    //     std::cout << attributeName + "-" + keyToRetrieve << std::endl;  
                    //     result[attributeName + "-" + keyToRetrieve] = ""; 
                    // }
                }
            }
        }
    }

    return result;
}

} // namespace dss_adcs
