#ifndef OPTION_URL_HPP
#define OPTION_URL_HPP

#include <iostream>
using namespace std;

class OptionURL {
    private: 
        std::string option_url;

    public:
        std::string getOptionUrl(const std::string& option);
        std::string getSportsWithParamURL(const std::string& param);
        std::string getEventOddsURL(const std::string& sport,const std::string& region, const std::string& market, const std::string eventId);
};

#endif 