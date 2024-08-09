#include <iostream>
#include "crow.h"
#include "get_function.hpp"
#include "sql_queries.hpp"
#include "curl_request.hpp"
#include "option_url.hpp"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });
    
    CROW_ROUTE(app, "/api/sports")
    .methods("GET"_method)([](const crow::request& req, crow::response& res){
        
        OptionURL bet_option;
        std::string option = "Sports";
        std::string url = bet_option.getOptionUrl(option);
        if(!url.empty()) {
            CROW_LOG_INFO << "Found URL: " << url;
        } else {
            CROW_LOG_ERROR << "Could not find proper url.";
        }
        const char* sports_url = url.c_str();
        json sports_data = getSports(sports_url);
        std::string sports = sports_data.dump(4);
        
        CROW_LOG_INFO << "Succeded the request.";
        res.set_header("Content-Type", "text/plain");
        res.set_header("Access-Control-Allow-Origin", "http://localhost:4200");
        res.set_header("Access-Control-Allow-Origin", "http://localhost:80");
        res.set_header("Access-Control-Allow-Origin", "http://localhost");
        res.set_header("Access-Control-Allow-Origin", "https://parsing-server-e2gdd0g5bycahwgd.eastus-01.azurewebsites.net/api");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.write(sports);
        res.end();

    });

  CROW_ROUTE(app, "/api/scores/<string>")
  .methods(crow::HTTPMethod::GET)
  ([](const crow::request& req, crow::response& res, const std::string& param){
    OptionURL sport_option;
    std::string url = sport_option.getSportsWithParamURL(param);

        if(!url.empty()) {
            CROW_LOG_INFO << "Found URL: " << url;
        } else {
            CROW_LOG_ERROR << "Could not find proper url.";
        }
    
    const char* scores_url = url.c_str();
    json sport_scores = getScores(scores_url);
    std::string scores = sport_scores.dump(4);
    res.set_header("Content-Type", "text/plain");
    res.set_header("Access-Control-Allow-Origin", "http://localhost:4200");
    res.set_header("Access-Control-Allow-Origin", "http://localhost:80");
    res.set_header("Access-Control-Allow-Origin", "http://localhost");
    res.set_header("Access-Control-Allow-Origin", "*");
    res.write(scores);
    res.end();
  });

  CROW_ROUTE(app, "/api/odds/<string>/<string>/<string>/<string>")
  .methods(crow::HTTPMethod::GET)
  ([](const crow::request& req,crow::response& res,
  const std::string& sport,
  const std::string& region,
  const std::string& market,
  const std::string eventId){
    OptionURL event_odds_option;
    std::string url = event_odds_option.getEventOddsURL(sport,region, market, eventId);
    
    if(!url.empty()) {
            CROW_LOG_INFO << "Found URL: " << url;
        } else {
            CROW_LOG_ERROR << "Could not find proper url.";
        }
    const char* event_odds_url = url.c_str();
    json event_odds = getEventOdds(event_odds_url);
    std::string odds = event_odds.dump(4);
    res.set_header("Content-Type", "text/plain");
    res.write(odds);
    res.end();
  });



    app.port(8080)
    .multithreaded()
    .run();


    return 0;
}