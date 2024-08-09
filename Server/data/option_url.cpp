#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>
#include <unordered_set>
#include "option_url.hpp"
#define base_url "https://api.the-odds-api.com/v4/sports/"

using namespace std;

std::string getApiKey() {
     const string apiKey = std::getenv("SPORTS_ODDS_API_KEY");
    if(!apiKey.empty()) {
        std::cout << "Key Found" << std::endl;
        return apiKey;
    } else {
        std::cerr << "Unable to find key" << std::endl;
        return "";
    }
}

std::string createBaseUrlParams(const std::string& sport_req, const std::string event_type) {
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
        {"nrl", "rugbyleague_nrl"}
        };

        
    std::string endpoint_base;
    if(sports.find(sport_req) != sports.end()) {
        endpoint_base += sports[sport_req];
    } else {
        std::cerr << "Error finding sport";
        return "";
    }

    std::string event_param_base;
    if(event_type == "odds") {
        event_param_base = "/odds/?apiKey=";
    } else if(event_type == "scores") {
        event_param_base = "/scores/?apiKey=";
    } else if(event_type =="event_odds") {
        event_param_base = "/events/{eventId}/odds?apiKey=";
    } else {
        std::cerr << "Invalid input\n";
        return "";
    }

    std::string apiKey = getApiKey();
    endpoint_base += event_param_base + apiKey;
    return endpoint_base;
    
}

std::string OptionURL::getOptionUrl(const std::string& option)
{
    map<string, string> scores = {
        {"NBA", "basketball_nba"},
        {"College Basketball", "basketball_ncaab"},
        {"NFL", "americanfootball_nfl"},
        {"College Football", "americanfootball_ncaaf"},
        {"MLB", "baseball_mlb"},
        {"NHL", "icehockey_nhl"},
        {"Boxing", "boxing_boxing"},
        {"MMA", "mma_mixed_martial_arts"},
        {"MLS", "soccer_usa_mls"}};
    string base = base_url;
    std::string apiKey = getApiKey();
    string url;
    string terminator = "\0";
    std::cout << option << " here is the options" << std::endl;
    if (option == "Sports")
    {
        base += "?apiKey=" + apiKey + terminator;
        option_url = base;
        return option_url;
    }
    else if (option == "Odds")
    {   char odds_choice;
        string sport_odds;
        std::cout << "\nWhat sport would you like to see the latest odds?\nEnter sports[s] to find a particular sport or all[a] for upcoming games: \n";
        cin>>odds_choice;
        cin.get();
        odds_choice = tolower(odds_choice);
        switch(odds_choice){
        case 's':
            for (auto it = scores.begin(); it != scores.end(); it++)
                {
                    std::cout << it->first << endl;
                }
                getline(cin, sport_odds);
                if (scores.find(sport_odds) != scores.end())
                {
                sport_odds = scores.find(sport_odds) ->second;
                base +=  sport_odds + "/odds/?apiKey=" + apiKey + terminator;
                url = base;
                }
                else{
                    cerr<<"could not find sport";
                }
        break;
        case 'a':
            base += "upcoming/odds/?apiKey=" + apiKey + terminator;
            url = base;
            
        break;
        default:
            std::cout<<"Incorrect choice made. Please try again";
            break;
        }
        
    }
    else if (option == "Scores")
    {
        std::cout << "Would you like to see live scores or completed games?\n";
        std::cout << "Please enter live or completed\n";
        string live;
        getline(cin, live);
        if (live == "completed")
        {
            live = "&daysFrom=2";
        }
        else
        {
            live = "";
        }
        std::cout << "\nWhat sport would you like to see the latest scores?\n";
        for (auto it = scores.begin(); it != scores.end(); it++)
        {
            std::cout << it->first << endl;
        }
        string sport_score;
        getline(cin, sport_score);
        if (scores.find(sport_score) != scores.end())
        {
            sport_score = scores.find(sport_score)->second;
            base += sport_score + "/scores/?apiKey=" + apiKey + live + terminator;
            url = base;
            
        }
        else
        {
            cerr << "Could not find sport";
        }
    }
    else
    {
        cerr << "Error: ";
    }

    return option_url;
}

std::string OptionURL::getSportsWithParamURL(const std::string& sport) {
    option_url = base_url;
    std::string event_type = "scores";
    std::string score_params = createBaseUrlParams(sport, event_type);
    std::string terminator = "\0";
    option_url += score_params + terminator;
    return option_url;
}

std::string OptionURL::getEventOddsURL(const std::string& sport,const std::string& region, const std::string& market, const std::string eventId) {
    option_url = base_url;
    std::string event_type = "odds";
    std::string odd_params = createBaseUrlParams(sport, event_type);
    std::cout << "Odd param: " << odd_params;  

    // Find the region of the event odds
    std::unordered_set<std::string> regions = {"us","us2","uk","eu","au"};
    std::string regionParam = "&regions=";
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
    std::string eventIdParam = "&eventIds=";
    eventIdParam.append(eventId);

    odd_params += regionParam + marketParam + oddsFormatParam +eventIdParam;
    option_url.append(odd_params);
    string terminator = "\0";
    option_url.append(terminator);
    std::cout << "Final URL: " << option_url << std::endl;
    return option_url;
}