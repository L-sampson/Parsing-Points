// #include <iostream>
// #include <string>
#include "curl_request.hpp"
using namespace std;

 size_t CurlRequest::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

struct curl_slist* CurlRequest::WriteCurlHeaders(const std::vector<std::string>& headers) {
    struct curl_slist* header_list =  NULL;
    for(const auto& header: headers) {
        header_list = curl_slist_append(header_list, header.c_str());
    }
    return header_list;
};


CURLcode CurlRequest::httpRequest(const char *view, string &response, const std::vector<std::string>& headersList)
{
    curl = curl_easy_init();
    CURLcode res;
    if(curl) {
        // Dynamically set headers
        struct curl_slist* header_list = WriteCurlHeaders(headersList);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, view);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

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