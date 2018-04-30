/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_API_CALL_HPP
#define DSS_ADCS_API_CALL_HPP

#include <dss_adcs/productSearch.hpp>

template < typename Identifier, typename Actuator >
void callTheApi(    Identifier actuatorUuid,
                    Actuator actuator )
{
    std::cout << "The API is being called to extract the parameters ... " << std::endl;   
} 

#endif // DSS_ADCS_API_CALL_HPP