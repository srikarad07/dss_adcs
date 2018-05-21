/*
 * Copyright (c) 2018, S.D. Cherukuri, Delft University of Technology (srikarad007@gmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
*/

#ifndef DSS_ADCS_API_CALL_HPP
#define DSS_ADCS_API_CALL_HPP

#include <iostream>

#include <cpr/cpr.h>

std::string callTheApi( const std::string actuatorUuid )
{
    const std::string apiUrl        = "https://api.satsearch.co/v1/products/"; 
    const std::string productUrl    = apiUrl + actuatorUuid;
    
    auto url = cpr::Url{ productUrl };
    
    auto headers = cpr::Header{ {"Authorization", "Bearer 0405ab03e7ba20b906b9f154fced17a4271ba92230e64c2e7cec5e30f9a8b660a0de6b1339ddbe3d2e45eb0ee9847c4alu8+41RxSWVaJpqS8XLxdqKqOC0y75jJOraWB0jYvlUIJgvmqEQhIVUGypazLrNO"}, {"X-APP-ID", "18a15813704205b3cf2ebeb364aa2ed246a9ed143b6911de4a52cb6045a0049ad1c951a6e8943e3dbc8c5c2c4242a447AbJtXh3J6aazkWKpCRz3nMut6Wu1YCWoEQESOin6qvkeqeDshqw0onjlA7dfM+3n"} };

    cpr::Session session;
    session.SetUrl(url);
    session.SetHeader(headers); 

    auto r = session.Get();

    // std::cout << r.url << std::endl; // http://www.httpbin.org/get
    // std::cout << r.status_code << std::endl; // 200
    std::cout << r.text << std::endl;

    return r.text; 
} 

#endif // DSS_ADCS_API_CALL_HPP