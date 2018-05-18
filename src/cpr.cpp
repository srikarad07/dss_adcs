#include <cpr/cpr.h>
#include <iostream>

int main(int argc, char** argv) {
    // auto r = cpr::Get(cpr::Url{"https://api.satsearch.co/v1/products/15b09050-b756-51e8-85ac-7d809f4a42ef"}, cpr::Header{ {"Authorization", "Bearer 0405ab03e7ba20b906b9f154fced17a4271ba92230e64c2e7cec5e30f9a8b660a0de6b1339ddbe3d2e45eb0ee9847c4alu8+41RxSWVaJpqS8XLxdqKqOC0y75jJOraWB0jYvlUIJgvmqEQhIVUGypazLrNO"}, {"X-APP-ID", "18a15813704205b3cf2ebeb364aa2ed246a9ed143b6911de4a52cb6045a0049ad1c951a6e8943e3dbc8c5c2c4242a447AbJtXh3J6aazkWKpCRz3nMut6Wu1YCWoEQESOin6qvkeqeDshqw0onjlA7dfM+3n"} } );
    
    // auto r = cpr::GetAsync(cpr::Url{"https://api.satsearch.co/v1/products/15b09050-b756-51e8-85ac-7d809f4a42ef"}, cpr::Header{ {"Bearer", "0405ab03e7ba20b906b9f154fced17a4271ba92230e64c2e7cec5e30f9a8b660a0de6b1339ddbe3d2e45eb0ee9847c4alu8+41RxSWVaJpqS8XLxdqKqOC0y75jJOraWB0jYvlUIJgvmqEQhIVUGypazLrNO"}, {"X-APP-ID", "18a15813704205b3cf2ebeb364aa2ed246a9ed143b6911de4a52cb6045a0049ad1c951a6e8943e3dbc8c5c2c4242a447AbJtXh3J6aazkWKpCRz3nMut6Wu1YCWoEQESOin6qvkeqeDshqw0onjlA7dfM+3n"} } );
    // r.get();
    // r.status_code;                  // 200
    // r.header["content-type"];       // application/json; charset=utf-8
    // r.text;                         // JSON text string
    const std::string productUrl = "https://api.satsearch.co/v1/products/"; 
    const std::string apiUrl  = productUrl + "1354b545-a497-5660-85a6-097f7cabb6b7";
    auto url = cpr::Url{apiUrl};
    auto headers = cpr::Header{ {"Authorization", "Bearer 0405ab03e7ba20b906b9f154fced17a4271ba92230e64c2e7cec5e30f9a8b660a0de6b1339ddbe3d2e45eb0ee9847c4alu8+41RxSWVaJpqS8XLxdqKqOC0y75jJOraWB0jYvlUIJgvmqEQhIVUGypazLrNO"}, {"X-APP-ID", "18a15813704205b3cf2ebeb364aa2ed246a9ed143b6911de4a52cb6045a0049ad1c951a6e8943e3dbc8c5c2c4242a447AbJtXh3J6aazkWKpCRz3nMut6Wu1YCWoEQESOin6qvkeqeDshqw0onjlA7dfM+3n"} };
    // // auto authentication = cpr::Authentication{ "Bearer","0405ab03e7ba20b906b9f154fced17a4271ba92230e64c2e7cec5e30f9a8b660a0de6b1339ddbe3d2e45eb0ee9847c4alu8+41RxSWVaJpqS8XLxdqKqOC0y75jJOraWB0jYvlUIJgvmqEQhIVUGypazLrNO"};
    // // auto header = cpr::Header{ {"X-APP-ID", "18a15813704205b3cf2ebeb364aa2ed246a9ed143b6911de4a52cb6045a0049ad1c951a6e8943e3dbc8c5c2c4242a447AbJtXh3J6aazkWKpCRz3nMut6Wu1YCWoEQESOin6qvkeqeDshqw0onjlA7dfM+3n"}  };
    cpr::Session session;
    session.SetUrl(url);
    session.SetHeader(headers);
    // session.SetOption(url); 
    // // session.SetAuth( authentication );
    // session.SetHeader(header); 

    auto r = session.Get();

    std::cout << r.url << std::endl; // http://www.httpbin.org/get
    std::cout << r.status_code << std::endl; // 200
    std::cout << r.text << std::endl;

}