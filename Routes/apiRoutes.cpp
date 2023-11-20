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
    //GET /v4/sports/{sport}/scores/?apiKey={apiKey}&daysFrom={daysFrom}&dateFormat={dateFormat}
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

    map<string, string> scores ={
    {"NBA", "basketball_nba"}, 
    {"NFL", "americanfootball_nfl"}, 
    {"MLB", "baseball_mlb"},
    {"NHL", "icehockey_nhl"},
    {"MMA", "mma_mixed_martial_arts"},
    {"MLS", "soccer_usa_mls"}};

    cout<<"Welcom to Parsing *Points\n";
    cout << "What sports data would you like to find today?\nChoose from the following options\n";
    cout << "Type Sports, Odds, Scores: ";
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
    else if(option=="Scores"){
        cout<<"\nWhat sport would you like to see the latest scores?\n";
        for(auto it = scores.begin(); it!= scores.end();it++){
            cout<<it->first<<endl;
        }
        string sport_score;
        getline(cin, sport_score);
        if(scores.find(sport_score) !=scores.end()){
            sport_score = scores.find(sport_score)->second;
            score+=sport_score + "/scores/?apiKey=" + apiKey + terminator;
            url = score;
        }
        else{
            cerr<<"Could not find sport";
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
void getScores(const char *aScores){
    string readScores;
    CURL *scores = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(scores, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(scores, CURLOPT_URL, aScores);
    curl_easy_setopt(scores, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(scores, CURLOPT_WRITEDATA, &readScores);

    res = curl_easy_perform(scores);
    if (res != CURLE_OK)
    {
        cout << "Error downloading web page: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(scores);
    }
    curl_easy_cleanup(scores);
    cout << "\nSuccess\n";
    parse(readScores);
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
        getOdds(url);
    }
    else if (option =="Scores")
    {
        getScores(view);
    }

    return 0;
}