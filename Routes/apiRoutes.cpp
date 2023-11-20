#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <string.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <map>
#define base_url "https://api.the-odds-api.com/v4/sports/?apiKey="
#define score_url "https://api.the-odds-api.com/v4/sports/"
#define odds_url "https://api.the-odds-api.com/v4/sports/upcoming/odds/?apiKey="

using namespace std;
using namespace nlohmann;

struct OptionURL
{
    string option;
    string url;
};

OptionURL envFile()
{

    ifstream envFile("../apiKey.env");
    // Check if the file is open
    if (!envFile.is_open())
    {
        return {"Error opening .env file"};
    }
    // Read the API key from the file
    string apiKey;
    getline(envFile, apiKey);
    if (apiKey.empty())
    {
        return {"API key not found in .env file"};
    }
    string base = base_url;
    string score = score_url;
    string oddLink = odds_url;
    string option;

    map<string, string> scores = {
        {"NBA", "basketball_nba"},
        {"NFL", "americanfootball_nfl"},
        {"MLB", "baseball_mlb"},
        {"NHL", "icehockey_nhl"},
        {"MMA", "mma_mixed_martial_arts"},
        {"MLS", "soccer_usa_mls"}};

    cout << "Welcome to Parsing *Points\n";
    cout << "What sports data would you like to find today?\nChoose from the following options\n";
    cout << "Sports, Odds, Scores: ";
    cout << "Please type in you selection now\n";

    string url;
    string terminator = "\0";
    getline(cin, option);
    if (option == "Sports")
    {
        base.append(apiKey);
        base.append(terminator);
        url = base;
        // cout<<url<<endl;
    }
    else if (option == "Odds")
    {
        oddLink.append(apiKey);
        oddLink.append(terminator);
        url = oddLink;
        // cout<<url<<endl;
    }
    else if (option == "Scores")
    {
        cout << "\nWhat sport would you like to see the latest scores?\n";
        for (auto it = scores.begin(); it != scores.end(); it++)
        {
            cout << it->first << endl;
        }
        string sport_score;
        getline(cin, sport_score);
        if (scores.find(sport_score) != scores.end())
        {
            sport_score = scores.find(sport_score)->second;
            score += sport_score + "/scores/?apiKey=" + apiKey + terminator;
            url = score;
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
    envFile.close();
    return {option, url};
}

void parse(string &readBuff)
{
    auto p = json::parse(readBuff);
    json pdata = p;
    for (int i = 0; i < pdata.size(); i++)
    {
        cout << pdata[i] << endl;
    }
}
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

CURLcode httpRequest(const char *view, string &response)
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, view);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        cout << "Error downloading web page: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
    }
    curl_easy_cleanup(curl);
    cout << "\nSuccess\n";
    return res;
}

// GET sports methods
void getSports(const char *aSports)
{
    string response;
    CURLcode res = httpRequest(aSports, response);
    if (res == CURLE_OK)
    {
        parse(response);
    }
}

// GET Odds Endpoint
void getOdds(string url)
{
    cout << "What country would you like to search odds for?\n";
    cout << "Enter one of the following options:\n";
    map<string, string> regions = {
        {"United States", "us"},
        {"United Kingdom", "uk"},
        {"Europen Union", "eu"},
        {"Australia", "au"}};
    cout << "Countries : Market\n";
    for (auto itr = regions.begin(); itr != regions.end(); itr++)
    {
        cout << itr->first << endl;
    }
    string region;
    string newReigon;
    getline(cin, region);

    if (regions.find(region) != regions.end())
    {
        newReigon = "&regions=";
        string choice = regions.find(region)->second;
        newReigon.append(choice);
    }
    else
    {
        cout << "Could not find market\n";
    }

    map<string, string> markets = {
        {"Money Line", "h2h"},
        {"Points", "spreads"},
        {"Over/Under", "totals"},
        {"Futures", "outrights"}};

    cout << "What betting options would you like to bet on?\n";
    for (auto itr = markets.begin(); itr != markets.end(); itr++)
    {
        cout << itr->first << endl;
    }

    string market;
    string newMarket;
    getline(cin, market);

    if (markets.find(market) != markets.end())
    {
        newMarket = "&markets=";
        string choice = markets.find(market)->second;
        newMarket.append(choice);
    }
    else
    {
        cout << "Could not find market\n";
    }
    string end = newReigon.append(newMarket);
    url.append(end);
    string terminator = "\0";
    url.append(terminator);
    //converting into char*
    const char *odds = url.c_str();
    string response;
    CURLcode res = httpRequest(odds, response);
    if (res == CURLE_OK)
    {
        parse(response);
    }
}

// GET Scores
void getScores(const char *aScores)
{
    string response;
    CURLcode res = httpRequest(aScores, response);
    if (res == CURLE_OK)
    {
        parse(response);
    }
    // string readScores;
    // CURL *scores = curl_easy_init();
    // CURLcode res;
    // curl_easy_setopt(scores, CURLOPT_CUSTOMREQUEST, "GET");
    // curl_easy_setopt(scores, CURLOPT_URL, aScores);
    // curl_easy_setopt(scores, CURLOPT_WRITEFUNCTION, WriteCallback);
    // curl_easy_setopt(scores, CURLOPT_WRITEDATA, &readScores);

    // res = curl_easy_perform(scores);
    // if (res != CURLE_OK)
    // {
    //     cout << "Error downloading web page: " << curl_easy_strerror(res) << endl;
    //     curl_easy_cleanup(scores);
    // }
    // curl_easy_cleanup(scores);
    // cout << "\nSuccess\n";
    // parse(readScores);
}
/*Stretch Goals:
Get Historical Odds
Get Event Odds
refactor to make code more DRY!!!*/

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