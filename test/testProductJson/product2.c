/********* Sample code generated by the curl command line tool **********
 * All curl_easy_setopt() options are documented at:
 * https://curl.haxx.se/libcurl/c/curl_easy_setopt.html
 ************************************************************************/
#include <curl/curl.h>

int main(int argc, char *argv[])
{
  CURLcode ret;
  CURL *hnd;
  struct curl_slist *slist1;

  slist1 = NULL;
  slist1 = curl_slist_append(slist1, "Authorization: Bearer 0405ab03e7ba20b906b9f154fced17a4271ba92230e64c2e7cec5e30f9a8b660a0de6b1339ddbe3d2e45eb0ee9847c4alu8+41RxSWVaJpqS8XLxdqKqOC0y75jJOraWB0jYvlUIJgvmqEQhIVUGypazLrNO");
  slist1 = curl_slist_append(slist1, "X-APP-ID: 18a15813704205b3cf2ebeb364aa2ed246a9ed143b6911de4a52cb6045a0049ad1c951a6e8943e3dbc8c5c2c4242a447AbJtXh3J6aazkWKpCRz3nMut6Wu1YCWoEQESOin6qvkeqeDshqw0onjlA7dfM+3n");

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_URL, "https://api.satsearch.co/v1/products/6454b937-f0e9-5fc5-8165-0cc79c44b424");
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.52.1");
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt");
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

  /* Here is a list of options the curl code used that cannot get generated
     as source easily. You may select to either not use them or implement
     them yourself.

  CURLOPT_WRITEDATA set to a objectpointer
  CURLOPT_INTERLEAVEDATA set to a objectpointer
  CURLOPT_WRITEFUNCTION set to a functionpointer
  CURLOPT_READDATA set to a objectpointer
  CURLOPT_READFUNCTION set to a functionpointer
  CURLOPT_SEEKDATA set to a objectpointer
  CURLOPT_SEEKFUNCTION set to a functionpointer
  CURLOPT_ERRORBUFFER set to a objectpointer
  CURLOPT_STDERR set to a objectpointer
  CURLOPT_HEADERFUNCTION set to a functionpointer
  CURLOPT_HEADERDATA set to a objectpointer

  */

  ret = curl_easy_perform(hnd);

  curl_easy_cleanup(hnd);
  hnd = NULL;
  curl_slist_free_all(slist1);
  slist1 = NULL;

  return (int)ret;
}
/**** End of sample code ****/
