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
/* Function call to extract the attributes from the reaction wheel JSON string. 
*  @param[in]    attributes         Attributes of the JSON string in rapidjson::Value   
*  @param[in]    findMemberName     String to compare the attribute with 
*  @param[in]    findMemberValues   std::vector of attributes names to be extracted. 
*  @param[in]    keysToRetreive     std::vector of attributes keys to be extracted. 
*  @param[out]   std::map< std::string attributeName, std::pair< std::string value, std::string measurementUnit > >
*/
std::map<std::string, std::pair<std::string,std::string>> mapForAttributeThatMatchesName(const rapidjson::Value& attributes, const std::string& findMemberName, const std::vector< std::string >findMemberValue, const std::vector<std::string>& keysToRetrieve) 
{   
    std::map<std::string, std::pair<std::string,std::string> > result;
    //! Save the names of all the attributes from the JSON string to compare with 
    //! the attributes in findMemberValue. 
    std::vector< std::string > tempAttributeNamesToSave; 

    for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) 
    {
        const rapidjson::Value& attributeClass = (*itr)["class"]; 
        for (rapidjson::Value::ConstMemberIterator itr2 = attributeClass.MemberBegin(); itr2 != attributeClass.MemberEnd(); ++itr2 )
        {  
            if ( findMemberName == itr2->name.GetString() && std::any_of( findMemberValue.cbegin(), findMemberValue.cend(), [&itr2]( std::string attributeString ){ return attributeString == itr2->value.GetString(); } ) ) 
            {   
                std::string attributeName = itr2->value.GetString(); 
                
                tempAttributeNamesToSave.push_back(attributeName);

                std::pair<std::string, std::string > attributeValueAndMeasurementUnit;
                std::string tempAttributeValue("ERROR!"), tempAttributeMeasurementUnit("ERROR!"); 

                // std::cout << attributeName << std::endl; 
                for (auto &keyToRetrieve : keysToRetrieve) 
                {
                    const rapidjson::Value::ConstMemberIterator currentAttributeToReturn = itr->FindMember(keyToRetrieve.c_str());
                    
                    const std::string currentAttributeValue = currentAttributeToReturn->value.GetString();

                    if (currentAttributeToReturn != itr->MemberEnd() && currentAttributeToReturn->value.IsString() && currentAttributeValue.compare("") != 0 ) 
                    {
                        // std::cout << "Map element test: " << attributeName + "-" + keyToRetrieve << std::endl;
                        if( keyToRetrieve.compare("value") == 0)
                        {
                            tempAttributeValue = currentAttributeToReturn->value.GetString();
                        }
                        else if(keyToRetrieve.compare("measurement_unit") == 0)
                        {
                            tempAttributeMeasurementUnit = currentAttributeToReturn->value.GetString();
                            std::cout << "Enters the normal loop: " << tempAttributeMeasurementUnit << std::endl; 
                        }
                        attributeValueAndMeasurementUnit = std::make_pair( tempAttributeValue, tempAttributeMeasurementUnit );

                        // std::cout << currentAttributeToReturn->value.GetString() << std::endl; 
                    }
                    else if ( currentAttributeValue.compare("") == 0 )
                    {
                        // std::cout << attributeName + "-" + keyToRetrieve << std::endl;  
                        std::string tempValue = "maximum_value"; 
                        const rapidjson::Value::ConstMemberIterator currentAttributeValueTemp = itr->FindMember(tempValue.c_str()); 
                        // result[attributeName + "-" + keyToRetrieve] = currentAttributeValueTemp->value.GetString();
                        // std::pair<std::string, std::string > attributeValueAndMeasurementUnit;
                        if( keyToRetrieve.compare("value") == 0)
                        {
                            tempAttributeValue = currentAttributeValueTemp->value.GetString();
                        }
                        else if(keyToRetrieve.compare("measurement_unit") == 0)
                        {
                            tempAttributeMeasurementUnit = currentAttributeValueTemp->value.GetString();
                            std::cout << "Enters the max loop: " << tempAttributeMeasurementUnit << std::endl; 
                        }
                        attributeValueAndMeasurementUnit = std::make_pair( tempAttributeValue, tempAttributeMeasurementUnit );

                        // std::cout << result[attributeName + "-" + keyToRetrieve] << std::endl; 
                    }
                }
                result[attributeName] = attributeValueAndMeasurementUnit;

            }
        }
    };

    //! Assign empty string for the attributes which are not present in the JSON string.
    for ( std::vector<std::string>::const_iterator savedAttributeNamesIterator = findMemberValue.cbegin();
          savedAttributeNamesIterator != findMemberValue.cend(); ++savedAttributeNamesIterator )
    {
        if ( 0 == (std::any_of( tempAttributeNamesToSave.begin(), tempAttributeNamesToSave.end(), [&savedAttributeNamesIterator]( std::string attributeString ){ return attributeString == *savedAttributeNamesIterator; } ) ) )
        {
            // std::cout << "Test name iterator: " << *savedAttributeNamesIterator << std::endl; 

            std::pair<std::string, std::string> emptyAttributes( "", "" );
            result[*savedAttributeNamesIterator] = emptyAttributes;
        } 
    }
    return result;
}

// Fast way to write from a file into a c++ string.
std::string getStringFromFile(std::ifstream& in) 
{
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

// Progress bar for bulk simulations. 
void progressBar( int maxProgress, int currentProgress )
{
    float progress           = ( 1.0 / maxProgress ) * (currentProgress + 1.0); 
    int barWidth = 70;
    if ( currentProgress == round(0.25 * maxProgress) || currentProgress == round(0.5 * maxProgress) || currentProgress == round(0.75 *maxProgress) || currentProgress == (maxProgress - 1) )
    {
        std::cout << "[";
        int pos = barWidth * progress - 1;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();
        std::cout << std::endl;   
    
    }
}

} // namespace dss_adcs
