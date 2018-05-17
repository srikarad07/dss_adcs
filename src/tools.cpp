/*
 * Copyright (c) 2017, K. Kumar, Delft University of Technology (me@kartikkumar.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <sstream>
#include <stdexcept>
#include <locale> 
#include <iostream>
// #include <algorithm>  
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

std::map<std::string, std::string> mapForAttributeThatMatchesName(const rapidjson::Value& attributes, const std::string& findMemberName, const std::vector< std::string >findMemberValue, const std::vector<std::string>& keysToRetrieve) 
// std::map<std::string, std::string> mapForAttributeThatMatchesName(const rapidjson::Value& attributes, const std::string& findMemberName, const std::string& findMemberValue, const std::vector<std::string>& keysToRetrieve)
{   int ii = 0; 
    std::map<std::string, std::string> result;
    for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) 
    {
        const rapidjson::Value::ConstMemberIterator currentAttribute = itr->FindMember(findMemberName.c_str());
        if (currentAttribute != itr->MemberEnd() && currentAttribute->value.IsString()) 
        {
            std::string attributeName = currentAttribute->value.GetString(); 
            if ( std::any_of( findMemberValue.cbegin(), findMemberValue.cend(), [&attributeName]( std::string attributeString ){ return attributeString == attributeName; } ) ) 
            {   
                // ++ii; 
                // std::cout << "The counter is: " << ii << std::endl; 
                // std::cout << currentAttribute->value.GetString() << std::endl; 
                for (auto &keyToRetrieve : keysToRetrieve) 
                {
                    const rapidjson::Value::ConstMemberIterator currentAttributeToReturn = itr->FindMember(keyToRetrieve.c_str());
                    if (currentAttributeToReturn != itr->MemberEnd() && currentAttributeToReturn->value.IsString()) 
                    {
                        // std::string tempString =  currentAttribute->value.GetString() + keyToRetrieve; 
                        // std::cout << tempString << std::endl;
                        result[attributeName + "-" + keyToRetrieve] = currentAttributeToReturn->value.GetString();
                    }
                }
                // return result;
            }
        }
    }
    return result;
}

// ProductAttributeStruct structForAttributeSearchThatMatchedNames( const rapidjson::Value& attributes, const std::string& findMemberName, const std::string& findMemberValue, const std::vector<std::string>& keysToRetrieve )
// {
//         ProductAttributeStruct result( 0.0, 0.0, 0.0, " " );
//         // ProductAttributeStruct result; 
//         for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) 
//         {
//         const rapidjson::Value::ConstMemberIterator currentAttribute = itr->FindMember(findMemberName.c_str());
//         if (currentAttribute != itr->MemberEnd() && currentAttribute->value.IsString()) 
//         {
//             if (currentAttribute->value == findMemberValue.c_str()) 
//             {

//                 for (auto &keyToRetrieve : keysToRetrieve) 
//                 {
//                     const rapidjson::Value::ConstMemberIterator currentAttributeToReturn = itr->FindMember(keyToRetrieve.c_str());
//                     std::string productAttributeNameValue   = currentAttributeToReturn->name.GetString(); 
//                     std::cout << "Attribute name: " << productAttributeNameValue << std::endl; 
//                     if (currentAttributeToReturn != itr->MemberEnd() && productAttributeNameValue.compare("measurementUnit") != 1 ) 
//                     {
//                         // result[keyToRetrieve] = currentAttributeToReturn->value.GetString();
//                         std::cout << currentAttributeToReturn->value.GetString() << std::endl;
//                     }
//                     else if (currentAttributeToReturn != itr->MemberEnd() && productAttributeNameValue.compare("value") != 1 )
//                     {
//                         std::cout << currentAttributeToReturn->value.GetDouble() << std::endl; 
//                     } 
//                     // if (currentAttributeToReturn != itr->MemberEnd() && currentAttributeToReturn->value.IsString() )
//                     // {
//                     //     result.measurementUnit = currentAttributeToReturn->value.GetString();
//                     //     std::cout << "Measurement loop " << std::endl; 
//                     // }
//                     // // else if ( currentAttributeToReturn != itr->MemberEnd() && currentAttributeToReturn->value.IsDouble() )
//                     // {
//                     //     result.value  = currentAttributeToReturn->value.GetDouble();
//                     //     std::cout << "value loop " << std::endl; 
//                     // }
//                 }
//                 // return result;
//             }
//         }
//     }
//     // ProductAttributeStruct result( 0.0, 0.0, productValue, measurementUnit ); 
//     return result; 
// }

} // namespace dss_adcs
