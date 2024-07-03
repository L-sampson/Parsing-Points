#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "option_url.hpp"
#define base_url "https://api.the-odds-api.com/v4/sports/"

using namespace std;

OptionURL envFile()
{

    ifstream envFile("./apiKey.env");
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

    std::cout << "Welcome to Parsing *Points\n";
    std::cout << "What sports data would you like to find today?\nChoose from the following options\n";
    std::cout << "Sports, Odds, Scores: \n";
    std::cout << "Please type in you selection now\n";

    string url;
    string terminator = "\0";
    getline(cin, option);
    if (option == "Sports")
    {
        base += "?apiKey=" + apiKey + terminator;
        // base.append(apiKey);
        // base.append(terminator);
        url = base;
        std::cout<<url<<endl;
    }
    else if (option == "Odds")
    {   char odds_choice;
        string sport_odds;
        std::cout << "\nWhat sport would you like to see the latest odds?\nEnter sports[s] to find a particular sport or all[a] for upcoming games: \n";
        cin>>odds_choice;
        cin.get();
        odds_choice = tolower(odds_choice);
        switch(odds_choice){
        case 's':
            for (auto it = scores.begin(); it != scores.end(); it++)
                {
                    std::cout << it->first << endl;
                }
                getline(cin, sport_odds);
                if (scores.find(sport_odds) != scores.end())
                {
                sport_odds = scores.find(sport_odds) ->second;
                base +=  sport_odds + "/odds/?apiKey=" + apiKey + terminator;
                url = base;
                std::cout<<url<<endl;
                }
                else{
                    cerr<<"could not find sport";
                }
        break;
        case 'a':
            base += "upcoming/odds/?apiKey=" + apiKey + terminator;
            url = base;
            std::cout<<url<<endl;
        break;
        default:
            std::cout<<"Incorrect choice made. Please try again";
            break;
        }
        
    }
    else if (option == "Scores")
    {
        std::cout << "Would you like to see live scores or completed games?\n";
        std::cout << "Please enter live or completed\n";
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
        std::cout << "\nWhat sport would you like to see the latest scores?\n";
        for (auto it = scores.begin(); it != scores.end(); it++)
        {
            std::cout << it->first << endl;
        }
        string sport_score;
        getline(cin, sport_score);
        if (scores.find(sport_score) != scores.end())
        {
            sport_score = scores.find(sport_score)->second;
            base += sport_score + "/scores/?apiKey=" + apiKey + live + terminator;
            url = base;
            std::cout<<url<<endl;
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