#include <iostream>
#include "crow.h"
#include "get_function.hpp"
#include "sql_queries.hpp"
#include "curl_request.hpp"
#include "option_url.hpp"

int main()
{
    crow::SimpleApp app;
    
    CROW_ROUTE(app, "/sports")
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
        res.write(sports);
        res.end();

    });

  CROW_ROUTE(app, "/scores/<string>")
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
    res.write(scores);
    res.end();
  });



    app.port(8080)
    .multithreaded()
    .run();


    return 0;
}