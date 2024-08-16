#include <iostream>
#include "option_url.hpp"
#define base_url "https://odds.p.rapidapi.com/v4/sports"

using namespace std;

std::string getApiKey() {
    try {
        const char* apiKey = std::getenv("SPORTS_ODDS_API_KEY");
            if(apiKey) {
                std::cout << "Key Found" << std::endl;
                return std::string(apiKey);
            } else {
                throw std::runtime_error("Unable to find API key");
        }
    } catch(std::exception& e) {
        std::cerr <<"Error: " << e.what() << std::endl;
        throw;
    }
}

std::string createBaseUrlParams(const std::string& sport_req ="", const std::string event_type="") {
    std::map<std::string, std::string> eventMap = {
        {"sports", "?all=false"},
        {"odds", "/odds?"},
        {"scores", "/scores"},
        {"event_odds", "/events/{eventId}/odds?apiKey="}
    };

    std::string event_param_base;
    if(eventMap.find(event_type) != eventMap.end()) {
        event_param_base = eventMap[event_type];
        if(event_type == "sports") {
            return event_param_base;
        }
    } else {
        throw std::runtime_error("Unable to find Endpoint base params");
    }
    
    map<string, string> sports = {
        {"nba", "basketball_nba"},
        {"wnba","basketball_wnba"},
        {"nccab", "basketball_ncaab"},
        {"nfl", "americanfootball_nfl"},
        {"nccaf", "americanfootball_ncaaf"},
        {"mlb", "baseball_mlb"},
        {"nhl", "icehockey_nhl"},
        {"boxing", "boxing_boxing"},
        {"mma", "mma_mixed_martial_arts"},
        {"mls", "soccer_usa_mls"}, 
        {"nrl", "rugbyleague_nrl"},
        {"upcoming", "upcoming"}
        };

        
    std::string endpoint_base;
    if(sports.find(sport_req) != sports.end()) {
        endpoint_base +="/" + sports[sport_req];
    } else {
        std::cerr << "Error finding sport";
        return "";
    }

    endpoint_base += event_param_base;
    return endpoint_base;
    
}

OptionURL::RequestData OptionURL::getRequestData(
    const std::string& option= "Sports", const std::string& sport_req = "", 
    const std::string& event_type = "", const std::string market_params = "") {
    RequestData data;

    if(option == "Sports") {
        data.option_url = base_url;
        if(event_type == "odds") {
            data.option_url += createBaseUrlParams(sport_req, event_type) + market_params + "\0";
        } else {
            data.option_url += createBaseUrlParams(sport_req, event_type) + "\0";
        }        
    }

    data.headers = getHeaders(option);
    return data;
}


std::string OptionURL::getEventOddsURL(const std::string& sport,const std::string& region, const std::string& market) {
    // option_url = base_url;
    // std::string event_type = "odds";
    // std::string odd_params = createBaseUrlParams(sport, event_type);
    // std::cout << "Odd param: " << odd_params;  

    // Find the region of the event odds
    std::unordered_set<std::string> regions = {"us","us2","uk","eu","au"};
    std::string regionParam = "regions=";
        if(regions.find(region) != regions.end()) {
            regionParam.append(region);
        } else {
            std::cerr << "Could find region parameter\n";
        }

    // Find the markets of the event odds
    std::string marketParam = "&markets=";
    std::unordered_set<std::string> markets = {"h2h","spreads", "totals" "outrights"};
         if(markets.find(market) != markets.end()) {
            marketParam.append(market);
            } else {
                std::cerr << "Could not find market parameter\n";
            }
    

    const std::string oddsFormatParam = "&oddsFormat=american";
    // std::string eventIdParam = "&eventIds=";
    // eventIdParam.append(eventId);

    std::string odd_params;
    odd_params += regionParam + marketParam + oddsFormatParam;
    // option_url.append(odd_params);
    // string terminator = "\0";
    // option_url.append(terminator);
    std::cout << "Final Odds Params: " << odd_params << std::endl;
    return odd_params;
}

std::vector<std::string> OptionURL::getHeaders(std::string header_option) {
    std::string apiKey = getApiKey();
    std::vector<std::string> headersList = {
        "x-rapidapi-key: " + apiKey,
        "x-rapidapi-host: "
    };

    if (header_option == "Sports") {
        headersList[1] = headersList[1] + "odds.p.rapidapi.com";
        return headersList;
    } else if(header_option == "News") {
        headersList[1] = headersList[1] + "flashlive-sports.p.rapidapi.com";
        return headersList;
    }
    throw std::runtime_error("Unable to find header host option");
}