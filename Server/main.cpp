#include<iostream>
#include "get_function.hpp"
#include "sql_queries.hpp"
#include "curl_request.hpp"
#include "option_url.hpp"

int main()
{
    OptionURL choice = envFile();
    string option = choice.option;
    string url = choice.url;
    const char *view = url.c_str();

    if (option == "Sports")
    {
        getSports(view);
    }
    else if (option == "Odds")
    {
        getOdds(view);
    }
    else if (option == "Scores")
    {
        getScores(view);
    }

    return 0;
}