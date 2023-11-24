#include<iostream>
#include<curl/curl.h>
#include<string>
#include "curl_request.hpp"
using namespace std;
CURLcode httpRequest(const char *view, string &response)
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, view);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        cout << "Error downloading web page: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
    }
    curl_easy_cleanup(curl);
    cout << "\nSuccess"<<CURLE_OK<<endl;;
    return res;
}