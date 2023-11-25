#include<iostream>
#include <fstream>
#include <string>
#include <map>
#include "option_url.hpp"
#define base_url "https://api.the-odds-api.com/v4/sports/"

using namespace std;

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
    string option;

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

    cout << "Welcome to Parsing *Points\n";
    cout << "What sports data would you like to find today?\nChoose from the following options\n";
    cout << "Sports, Odds, Scores: \n";
    cout << "Please type in you selection now\n";

    string url;
    string terminator = "\0";
    getline(cin, option);
    if (option == "Sports")
    {
        base += "?apiKey=" + apiKey + terminator;
        // base.append(apiKey);
        // base.append(terminator);
        url = base;
        cout<<url<<endl;
    }
    else if (option == "Odds")
    {
        base += "upcoming/odds/?apiKey=" + apiKey + terminator;
        url = base;
        cout<<url<<endl;
    }
    else if (option == "Scores")
    {
        cout << "Would you like to see live scores or completed games?\n";
        cout << "Please enter live or completed\n";
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
            base += sport_score + "/scores/?apiKey=" + apiKey + live + terminator;
            url = base;
            cout<<url<<endl;
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