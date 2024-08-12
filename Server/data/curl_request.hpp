#ifndef CURL_REQUEST_HPP
#define CURL_REQUEST_HPP


#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

class CurlRequest {
    public:
    CURLcode httpRequest(const char *view, std::string &response, const std::vector<std::string>& headersList);

    private:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    struct curl_slist* WriteCurlHeaders(const std::vector<std::string>& headers);
    CURL* curl;
};

#endif // CURL_REQUEST_HPP
