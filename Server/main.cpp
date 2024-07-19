#include <iostream>
#include "crow.h"
#include "get_function.hpp"
#include "sql_queries.hpp"
#include "curl_request.hpp"
#include "option_url.hpp"
#define base_url "https://api.the-odds-api.com/v4/sports/"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/sports")
    .methods("GET"_method)([](const crow::request& req, crow::response& res){
        std::string response;
        std::string base = base_url;
        std::string terminator = "\0";
        const string apiKey = std::getenv("SPORTS_ODDS_API_KEY");
        base += "?apiKey=" + apiKey + terminator;
        std::string url;
        url = base;
        const char* sports_url = url.c_str();
        CurlRequest sportsRequest;
        CURLcode curl_response = sportsRequest.httpRequest(sports_url, response);
        if(curl_response != CURLE_OK) {
            res.write("Error fetching data from sports API");
            res.write(curl_easy_strerror(curl_response));
            return;
        }

        res.set_header("Content-Type", "text/plain");
        res.set_header("Access-Control-Allow-Origin", "http://localhost:4200");
        res.write(response);
        res.end();

    });

    app.port(8080)
    .multithreaded()
    .run();


    return 0;
}