#ifndef CURL_REQUEST_HPP
#define CURL_REQUEST_HPP


#include <iostream>
#include <string>
#include <curl/curl.h>

class CurlRequest {
    public:
    CURLcode httpRequest(const char *view, std::string &response);

    private:
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    CURL* curl;
};

#endif // CURL_REQUEST_HPP
