#include <iostream>
#include <string>
#include "curl_request.hpp"
using namespace std;

 size_t CurlRequest::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

CURLcode CurlRequest::httpRequest(const char *view, string &response)
{
    curl = curl_easy_init();
    CURLcode res;
    if(curl) {
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
    } else {
    // Handle error - couldn't initialize CURL
    cerr << "Failed to initialize CURL" << endl;
    res = CURLE_FAILED_INIT;
  }
    
    curl_easy_cleanup(curl);
    cout << "\nSuccess"<<endl;;
    return res;
}