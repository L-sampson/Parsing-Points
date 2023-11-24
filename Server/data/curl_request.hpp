#include<iostream>
#include<string>
#include<curl/curl.h>

extern size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

CURLcode httpRequest(const char *view, std::string &response);
