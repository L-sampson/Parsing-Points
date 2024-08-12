#include <iostream>
#include "crow.h"
#include "get_function.hpp"
#include "sql_queries.hpp"
#include "curl_request.hpp"
#include "option_url.hpp"

void sendResponse(crow::response& res, const std::string& response_data, bool is_success) {
    if (is_success) {
        CROW_LOG_INFO << "Succeeded the request.";
        res.set_header("Content-Type", "text/plain");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.write(response_data);
        res.end();
    } else {
        CROW_LOG_INFO << "Failed request.";
        res.code = 404;
        res.write("Not found");
        res.end();
    }
}

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/sports")
    .methods("GET"_method)([](const crow::request& req, crow::response& res){
        
        OptionURL sport_option;
        OptionURL::RequestData requestData = sport_option.getRequestData("Sports", "", "sports", "");
        
         if (requestData.option_url.empty()) {
                CROW_LOG_ERROR << "Could not find proper url";
                sendResponse(res, "", false);
                return;
        }
        CROW_LOG_INFO << "Found URL: " << requestData.option_url;
        const char* sports_url = requestData.option_url.c_str();
        json sports_data = getSports(sports_url, requestData.headers);
        std::string sports_response = sports_data.dump(4);

        sendResponse(res, sports_response, !sports_response.empty());
    });

    CROW_ROUTE(app, "/api/scores")
    .methods(crow::HTTPMethod::GET)
    ([](const crow::request& req, crow::response& res){
        CROW_LOG_INFO << "Params: " << req.url_params << "\n\n";
        std::string sport_req = req.url_params.get("sport");
        OptionURL scores_option;
        OptionURL::RequestData requestData = scores_option.getRequestData("Sports", sport_req, "scores", "");
        if (requestData.option_url.empty()) {
                CROW_LOG_ERROR << "Could not find proper url";
                sendResponse(res, "", false);
                return;
            }

        CROW_LOG_INFO << "Found URL: " << requestData.option_url;
        const char* scores_url = requestData.option_url.c_str();
        json sports_data = getScores(scores_url, requestData.headers);
        std::string scores_response = sports_data.dump(4);

        sendResponse(res, scores_response, !scores_response.empty());
  });

  CROW_ROUTE(app, "/api/odds")
  .methods(crow::HTTPMethod::GET)
  ([](const crow::request& req,crow::response& res){
    std::string sport_req = req.url_params.get("sport");
    std::string region_req = req.url_params.get("region");
    std::string market_req = req.url_params.get("market");
    CROW_LOG_INFO << "Params: " << req.url_params << "\n\n";

    OptionURL odds_option;
    std::string market_params = odds_option.getEventOddsURL(sport_req, region_req, market_req);
    OptionURL::RequestData requestData = odds_option.getRequestData("Sports", sport_req, "odds", market_params);
    CROW_LOG_INFO << "URL: " << requestData.option_url << "\n";

        if (requestData.option_url.empty()) {
            CROW_LOG_ERROR << "Could not find proper url";
            sendResponse(res, "", false);
            return;
            }

    CROW_LOG_INFO << "Found URL: " << requestData.option_url;
    const char* odds_url = requestData.option_url.c_str();
    json sports_data = getEventOdds(odds_url, requestData.headers);
    std::string odds_response = sports_data.dump(4);

    sendResponse(res, odds_response, !odds_response.empty());

  });



    app.port(8080)
    .multithreaded()
    .run();


    return 0;
}