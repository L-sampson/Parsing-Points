#ifndef OPTION_URL_HPP
#define OPTION_URL_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <unordered_set>
using namespace std;

class OptionURL {
    private: 
        std::vector<std::string> headers;

    public:

        struct RequestData {
            std::string option_url;
            std::vector<std::string> headers;
        };

        RequestData getRequestData(const std::string& option, const std::string& sport_req, const std::string& event_type, const std::string market_params);
        std::string getEventOddsURL(const std::string& sport,const std::string& region, const std::string& market);
        std::vector<std::string> getHeaders(std::string header_option);
};

#endif 