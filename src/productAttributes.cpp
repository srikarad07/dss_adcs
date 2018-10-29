/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/
#include <fstream>
#include <iostream>
#include <math.h> 
#include <stdexcept>
#include <vector>

#include <sml/sml.hpp>

#include "dss_adcs/apiCall.hpp"
#include "dss_adcs/productAttributes.hpp"
#include "dss_adcs/tools.hpp"
#include "dss_adcs/getReactionWheelAttribute.hpp"
#include "dss_adcs/getReactionWheelAttributesInSIUnits.hpp"
#include "dss_adcs/typedefs.hpp"

namespace dss_adcs
{

void executeProductAttributeExtraction( const rapidjson::Document& config )
{
    //! Extract the actuator uuids for the products required for analysis. 
    ConfigIterator actuatorUuidIterator         = find( config, "actuator_uuids" ); 
    const rapidjson::Value& actuatorUuidSize 	= config["actuator_uuids"]; 
    std::vector< std::string > actuatorUuids; 

    for( unsigned int iterator = 0; iterator < actuatorUuidSize.Size(); ++iterator ) 
    {   
        actuatorUuids.push_back( actuatorUuidIterator->value[iterator].GetString() ); 
        // std::cout << actuatorUuids[iterator] << std::endl; 
    }; 

    //! Extract the attribtues to retrieve for the products. 
    ConfigIterator attributesToRetrieveIterator        = find( config, "attributes_to_retrieve" );
    const rapidjson::Value& attributesToRetrieveSize   = config["attributes_to_retrieve"];
    std::vector< std::string > attributesToRetrieve; 

    for( unsigned int iterator2 = 0; iterator2 < attributesToRetrieveSize.Size(); ++iterator2 )   
    {
        attributesToRetrieve.push_back( attributesToRetrieveIterator->value[iterator2].GetString() ); 
    }; 

	// Get the path to save the metadata parameters. 
	std::ofstream metadatHistoryStream( find( config, "metadataPath" )->value.GetString() ); 

	metadatHistoryStream << "name,supplier,angularMomentum,angularSpeedControlAccuracy,angularSpeed,connectorType,dataInterface,diameter,dynamicValue,height,idlePower,integratedElectronics,length,lifetime,mass,maxAngularSpeed,maxMomentumStorage,maxPower,maxTorque,maxVoltage,mechanicalInterface,mechanicalVibration,moi,operatingTemperature,powerInterface,radiationTolerance,staticUnbalance,steadyStatePower,storageTemperature,survivalTemperature,torque,voltageDc,voltage,width" << std::endl; 

    // Get the product json using the api.
    for( unsigned int actuatorIterator2 = 0; actuatorIterator2 < actuatorUuids.size(); ++actuatorIterator2 )
    {
        std::string reactionWheelString  = callTheApi( actuatorUuids[actuatorIterator2] );  
        
        rapidjson::Document config;
	    config.Parse<0>(reactionWheelString.c_str()).HasParseError();
	    assert(config.IsObject()); 

	    const rapidjson::Value& attributes 	= config["attributes"];
	    assert(attributes.IsArray()); 

        const std::string reactionWheelName 	= config["name"].GetString();
	    const std::string supplierName 			= config["supplier_name"].GetString();

	    std::vector<std::string> keysToRetrieve = {"measurement_unit", "value"};

    	std::map<std::string, std::pair<std::string, std::string> > mapForResult = mapForAttributeThatMatchesName( attributes, "name", attributesToRetrieve, keysToRetrieve );	 

        metadatHistoryStream << reactionWheelName       << "," 
                             << supplierName            << "," ; 
        std::pair< std::string, std::string > test  = mapForResult["torque"]; 
        // std::cout << "test value: " << test.first << " unit " << test.second << std::endl;  
        for ( std::map<std::string, std::pair<std::string,std::string> >::iterator mapIterator = mapForResult.begin(); mapIterator != mapForResult.end(); ++mapIterator )
        {
            std::pair<std::string, std::string> attributeValueAndUnit = mapIterator->second; 
            // std::cout << mapIterator->first << " value: " << attributeValueAndUnit.first << " unit: " << attributeValueAndUnit.second << std::endl; 
            metadatHistoryStream << attributeValueAndUnit.first << ","; 
        }
        metadatHistoryStream << std::endl; 
        // // Extract the reaction wheel properties from the map into SI Units. 
		// const ReactionWheel reactionWheel = getReactionWheelAttributesInSiUnits( mapForResult, 
		// 																	 	 reactionWheelName,
		// 										       						 	 supplierName ); 

		// double volume( 0.0 ); 

		// if ( !isnan( reactionWheel.radius ) ) 
        // {
		// 	volume += sml::SML_PI * reactionWheel.radius * reactionWheel.radius *reactionWheel.height; 
        // }
        // else if ( !isnan( reactionWheel.length ) && !isnan( reactionWheel.width ) )
        // {
        //     volume += reactionWheel.length * reactionWheel.height * reactionWheel.width; 
        // }

		// metadatHistoryStream 	<< reactionWheel.name				<< "," 
		// 				 		<< reactionWheel.supplierName		<< ","
		// 						<< reactionWheel.mass				<< ","
		// 						<< reactionWheel.length 			<< ","
		// 						<< reactionWheel.height 			<< ","
		// 						<< reactionWheel.width 				<< ","
		// 						<< reactionWheel.radius 			<< ","
		// 						<< volume							<< ","
		// 						<< reactionWheel.maxTorque			<< "," 
		// 						<< reactionWheel.maxMomentumStorage	<< ","
        //                         << reactionWheel.peakPower          << std::endl; 
    }
}

}