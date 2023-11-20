#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <string.h>
#include <nlohmann/json.hpp>
#include <vector>
#include<map>
#define base_url "https://api.the-odds-api.com/v4/sports/?apiKey="
#define odds_url "https://api.the-odds-api.com/v4/sports/upcoming/odds/?apiKey="

using namespace std;
using namespace nlohmann;
// creating Api routes in the folder

string envFile()
{
    ifstream envFile("../apiKey.env");
    // Check if the file is open
    if (!envFile.is_open())
    {
        return "Error opening .env file";
    }
    // Read the API key from the file
    string apiKey;
    getline(envFile, apiKey);
    if (apiKey.empty())
    {
        return "API key not found in .env file";
    }
    string base = base_url;
    string oddLink = odds_url;
    
    string option;
    
    
    cout<<"Do you want to find avaiable sports or odds today?\n";
    cout<<"Type Sports or Odds: ";
    string url;
    string terminator = "\0";
    getline(cin, option);
    if(option =="Sports"){
        string url = base;
        url.append(apiKey);
        url.append(terminator);
        cout<<url<<endl;
    }
    else if(option =="Odds")
    {
        url = oddLink;
        url.append(apiKey);
        cout<<url<<endl;
    }
    else{
        cout<<"Choose a valid option";
    }
    envFile.close();
    return (url);
}

void parse(string &readBuff)
{
    auto p = json::parse(readBuff);
    json pdata = p;
    for(int i = 0; i<pdata.size(); i++){
        cout<<pdata<<endl;
    }
}
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// GET sports methods
void getSports(const char *buff)
{
    string readSports;
    CURL *sports = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(sports, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(sports, CURLOPT_URL, buff);
    char *effectiveURL;
    curl_easy_getinfo(sports, CURLINFO_EFFECTIVE_URL, &effectiveURL);
    cout<<"Acutal URL: "<<effectiveURL<<endl;
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
    // return readBuff;
    // fix
    parse(readSports);
}

// GET Odds Endpoint
void getOdds()
{
    string readOdds;
    CURL *odds = curl_easy_init();
    CURLU *odds_handle;
    CURLcode res;

    if (odds)
    {
        odds_handle = curl_url();
    }
    string url = envFile();
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

    if(regions.find(region)!=regions.end()){
        newReigon= "&regions=";
        string choice = regions.find(region)->second;
        newReigon.append(choice); 
    }
    else{
        cout<<"Could not find market\n";
    }

    map<string, string> markets={
        {"Money Line", "h2h"},
        {"Points", "spreads"},
        {"Over/Under", "totals"},
        {"Futures", "outrights"}
    };

    cout<<"What betting options would you like to bet on?\n";
    for(auto itr = markets.begin(); itr != markets.end(); itr++){
        cout<<itr->first<<" : "<<itr->second<<endl;
    }

    string market;
    string newMarket;
    getline(cin, market);

    if(markets.find(market)!=markets.end()){
        newMarket = "&markets=";
        string choice = markets.find(market)->second;
        newMarket.append(choice);
    }
    else{
        cout<<"Could not find market\n";
    }
    string end = newReigon.append(newMarket);
    url.append(end);
    string terminator = "\0";
    url.append(terminator);
    const char *sport = url.c_str(); 
    // vector<char>sport(url.begin(), url.end());
    // for(auto itr = sport.begin(); itr!= sport.end(); itr++){
    //     cout<<*itr;
    // }
    // curl_url_set(odds_handle, CURLUPART_URL, sport.data(), 0);
    // curl_url_set(odds_handle, CURLUPART_PATH, sport, 0);
    curl_easy_setopt(odds, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(odds,CURLOPT_URL, sport);
    char *effectiveURL;
    curl_easy_getinfo(odds, CURLINFO_EFFECTIVE_URL, &effectiveURL);
    cout<<"Acutal URL: "<<effectiveURL<<endl;
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
    // curl_url_cleanup(odds_handle);
    parse(readOdds);
}

// GET Scores

// GET Historical odds

// GET event odds

int main()
{
    string url, option = envFile();
    // const char *availSports = url.c_str();
    // cout<<availSports;

    // vector<char> getUrl(url.begin(), url.end());
    // char buffer[getUrl.size() + 1];
    // // Copy the vector contents to the buffer
    // std::copy(getUrl.begin(), getUrl.end(), buffer);

    // // Add the null terminator
    // buffer[getUrl.size()] = '\0';

    //getOdds();

    // Pass the buffer to the function
    //option =="Sports" ?  getSports(availSports):  getOdds();
    
    
}