#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <string.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <map>
#define base_url "https://api.the-odds-api.com/v4/sports/?apiKey="
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
    string oddLink = odds_url;
    string option;

    cout << "Do you want to find avaiable sports or odds today?\n";
    cout << "Type Sports or Odds: ";
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

// GET sports methods
void getSports(const char *aSports)
{
    string readSports;
    CURL *sports = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(sports, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(sports, CURLOPT_URL, aSports);
    curl_easy_setopt(sports, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(sports, CURLOPT_WRITEDATA, &readSports);

    res = curl_easy_perform(sports);
    if (res != CURLE_OK)
    {
        cout << "Error downloading web page: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(sports);
    }
    curl_easy_cleanup(sports);
    cout << "\nSuccess\n";
    parse(readSports);
}

// GET Odds Endpoint
void getOdds(string url)
{
    string readOdds;
    CURL *odds = curl_easy_init();
    CURLU *odds_handle;
    CURLcode res;

    if (odds)
    {
        odds_handle = curl_url();
    }

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
        cout << itr->first << " : " << itr->second << endl;
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
        cout << itr->first << " : " << itr->second << endl;
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
    const char *sport = url.c_str();

    curl_easy_setopt(odds, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(odds, CURLOPT_URL, sport);
    char *effectiveURL;
    curl_easy_getinfo(odds, CURLINFO_EFFECTIVE_URL, &effectiveURL);
    cout << "Acutal URL: " << effectiveURL << endl;
    curl_easy_setopt(odds, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(odds, CURLOPT_WRITEDATA, &readOdds);
    cout << "\nSuccess:\n";
    res = curl_easy_perform(odds);
    if (res != CURLE_OK)
    {
        cerr << "Error downloding website page: " << curl_easy_strerror(res);
        curl_easy_cleanup(odds);
    }

    curl_easy_cleanup(odds);
    parse(readOdds);
}

// GET Scores

// GET Historical odds

// GET event odds

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
        getOdds(url);
    }
    else
    {
        cout << "Sorry" << endl;
    }

    return 0;
}