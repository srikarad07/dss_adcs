#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

std::map<std::string, std::string> mapForAttributeThatMatchesName(const rapidjson::Value& attributes, const std::string& findMemberName, const std::string& findMemberValue, const std::vector<std::string>& keysToRetrieve) 
{

    std::map<std::string, std::string> result;
    for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) {

        const rapidjson::Value::ConstMemberIterator currentAttribute = itr->FindMember(findMemberName.c_str());
        if (currentAttribute != itr->MemberEnd() && currentAttribute->value.IsString()) {

            if (currentAttribute->value == findMemberValue.c_str()) {

                for (auto &keyToRetrieve : keysToRetrieve) {

                    const rapidjson::Value::ConstMemberIterator currentAttributeToReturn = itr->FindMember(keyToRetrieve.c_str());
                    if (currentAttributeToReturn != itr->MemberEnd() && currentAttributeToReturn->value.IsString()) {

                        result[keyToRetrieve] = currentAttributeToReturn->value.GetString();
                    }
                }
                return result;
            }
        }
    }
    return result;
}

int main()
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
	// for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) 
	// {
    // 	const rapidjson::Value& attribute = *itr;
    // 	assert(attribute.IsObject()); // each attribute is an object
    // 	for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) 
	// 	{
	// 		counter++;  	
	// 		// std::cout << "The counter is: "<< counter << std::endl; 
			
    //     	std::cout << itr2->name.GetString() << " : " << itr2->	.GetString() << std::endl;
	// 	}
	// }

	
	return EXIT_SUCCESS; 
}



