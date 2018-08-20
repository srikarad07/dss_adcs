/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_PRODUCT_ATTRIBUTES_HPP
#define DSS_ADCS_PRODUCT_ATTRIBUTES_HPP

#include <string>
#include <vector>

#include <rapidjson/document.h>

namespace dss_adcs
{
void executeProductAttributeExtraction( const rapidjson::Document& config );

// struct ProductAttributeExtractorInput
// {

// public: 
//     ProductAttributeExtractorInput( const std::vector <std::string> aActuatorUuids, 
//                                     const std::vector <std::string> aAttributesToRetrieve )
//         :   actuatorUuids( actuatorUuids ), 
//             attributesToRetrieve( aAttributesToRetrieve )
//         { }
    
//     //! std::vector with actuator Uuids 
//     const std::vector< std::string > actuatorUuids; 

//     //! std::vector with attributes to retrieve. 
//     const std::vector< std::string > attributesToRetrieve; 

// };

}

#endif 