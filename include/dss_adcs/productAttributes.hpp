/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef DSS_ADCS_PRODUCT_ATTRIBUTES_HPP
#define DSS_ADCS_PRODUCT_ATTRIBUTES_HPP

#include <rapidjson/document.h>

namespace dss_adcs 
{

void executeProductAttributeExtraction( const rapidjson::Document& config ); 

} // namespace 

#endif 