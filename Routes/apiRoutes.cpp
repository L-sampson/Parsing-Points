#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <string.h>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <vector>
#include <map>
#include <set>

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
            score += sport_score + "/scores/?apiKey=" + apiKey + live + terminator;
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

/*void parse(string &readBuff)
{
    auto p = json::parse(readBuff);
    json pdata = p;

    for (int i = 0; i < pdata.size(); i++)
    {
        string home = pdata[i]["home_team"];
        string away = pdata[i]["away_team"];
        string sport = pdata[i]["sport_title"];
        cout << sport << endl;
        cout << home << " vs " << away << endl;
        for (auto &bookmaker : pdata[i]["bookmakers"])
        {
            cout << bookmaker["title"] << endl;
            for (auto &market : bookmaker["markets"])
            {
                cout << market["last_update"] << endl;
                for (auto &outcome : market["outcomes"])
                {
                    cout << outcome["name"] << " : " << outcome["price"] << endl;
                }
            }
        }
        cout << "\n";
    }
}*/
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
        map<string, string> sports;
        vector<bool> active;
        string status;
        json league_data = json::parse(response);
        for (int i = 0; i < league_data.size(); i++)
        {
            sports["League Name"] = league_data[i]["title"];
            sports["League Key"] = league_data[i]["key"];
            sports["Sport Group"] = league_data[i]["group"];
            sports["Description"] = league_data[i]["description"];
            active.push_back(league_data[i]["active"]);
            for (auto it = sports.begin(); it != sports.end(); it++)
            {
                cout << it->first << " : " << it->second << endl;
            }
            if (active[i])
            {
                status = "Active"; 
            }
            else
            {
                status = "Inactive";
            }
            cout << status << endl;
            cout << endl;
            pqxx::connection conn("dbname = sports user = postgres");

            if (conn.is_open())
            {
                cout << "Database opened: " << conn.dbname() << endl;
                pqxx::work l_table(conn);

                string insertQuery = "INSERT INTO leagues (LeagueName, LeagueKey, SportsType, Description, active) VALUES ( " 
                + l_table.quote(sports["League Name"]) + ", " 
                + l_table.quote(sports["League Key"]) + ", " 
                + l_table.quote(sports["Sport Group"]) + ", " 
                + l_table.quote(sports["Description"]) + ", "
                + l_table.quote(status) + ")";

                l_table.exec(insertQuery);
                l_table.commit();
                cout << "Data Entered" << endl;
                conn.disconnect();
            }
            else
            {
                cerr << "Can't open database" << endl;
            }
        }
    }
}

// GET Odds Endpoint
//Add sports Title.
void getOdds(string url)
{
    cout << "\nWhat country would you like to search odds for?\n";
    cout << "Enter one of the following options:\n";
    map<string, string> regions = {
        {"United States", "us"},
        {"United Kingdom", "uk"},
        {"Europen Union", "eu"},
        {"Australia", "au"}};
    cout << "\nCountries : Market\n";
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

    cout << "\nWhat betting options would you like to bet on?\n";
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
    // converting into char*
    const char *odds = url.c_str();
    string response;
    CURLcode res = httpRequest(odds, response);
    if (res == CURLE_OK)
    {
        map<string, string> game_odds;
        string book;
        // vector<string> bookmaker_title;
        string favorite;
        string underdog;
        string draw;
        double favoritePrice;
        double underdogPrice;
        json odds_data = json::parse(response);
        for (int i = 0; i < odds_data.size(); i++)
        {
            game_odds["Event ID"] = odds_data[i]["id"];
            game_odds["Home Team"] = odds_data[i]["home_team"];
            game_odds["Away Team"] = odds_data[i]["away_team"];
            game_odds["League"] = odds_data[i]["sport_title"];
            game_odds["Game Date"] = odds_data[i]["commence_time"];
            // cout << eventid << endl;
            // cout << sport << endl;
            // cout << home << " vs " << away << endl;
            favoritePrice = numeric_limits<double>::max();
            underdogPrice = numeric_limits<double>::min();
            for (auto it = game_odds.begin(); it != game_odds.end(); it++)
            {
                cout << it->first << " : " << it->second << endl;
            }
            for (auto &bookmaker : odds_data[i]["bookmakers"])
            {
                book = bookmaker["title"];

                for (auto &market : bookmaker["markets"])
                {

                    for (auto &outcome : market["outcomes"])
                    {
                        if (outcome["price"] < favoritePrice)
                        {
                            favoritePrice = outcome["price"];
                            favorite = outcome["name"];
                        }
                        else if (outcome["price"] > underdogPrice)
                        {
                            underdog = outcome["name"];
                            underdogPrice = outcome["price"];
                        }
                    }
                }
            }
            cout << book << endl;
            cout << favorite << " : " << favoritePrice << endl;
            cout << underdog << " : " << underdogPrice << endl;
            cout << "\n";
            pqxx::connection conn("dbname = sports user = postgres");
            if (conn.is_open())
            {
                cout << "Database is open: " << conn.dbname() << endl;
                pqxx::work o_data(conn);

                string insertQuery = "INSERT INTO odds (OddsID, Bookmaker, Date, FRprice, underprice, FrontRunner, Longshot) VALUES(" + o_data.quote(game_odds["Event ID"]) + ", " + o_data.quote(book) + ", " + o_data.quote(game_odds["Game Date"]) + ", " + o_data.quote(favoritePrice) + ", " + o_data.quote(underdogPrice) + ", " + o_data.quote(favorite) + ", " + o_data.quote(underdog) + ")";
                o_data.exec(insertQuery);
                o_data.commit();
                cout << "Data inserted" << endl;
                conn.disconnect();
            }
            else
            {
                cout << "Can't open database" << endl;
            }
        }
    }
}

// GET Scores
void getScores(const char *aScores)
{
    string response;
    CURLcode res = httpRequest(aScores, response);
    if (res == CURLE_OK)
    {
        // Create a Map that links the scores and teams.
        json score_data = json::parse(response);
        for (int i = 0; i < score_data.size(); i++)
        {
            map<string, string> game_data; // Create a new map for each game
            vector<bool> isGame_over;
            string game_over;
            // Extract data from the current game
            json current_game = score_data[i];
            game_data["League"] = current_game["sport_title"];
            game_data["Start Time"] = current_game["commence_time"];
            game_data["Home Team"] = current_game["home_team"];
            game_data["Away Team"] = current_game["away_team"];
            game_data["ID(event)"] = current_game["id"];
            isGame_over.push_back(current_game["completed"]);

            vector<pair<string, string>> scoreboard;
            json game_scores = score_data[i]["scores"];
            for (int i = 0; i < game_scores.size(); i++)
            {

                pair<string, string> score(game_scores[i]["name"], game_scores[i]["score"]);
                scoreboard.push_back(score);
                // cout<<game_scores[i]["name"]<<" : "<<game_scores[i]["score"]<<endl;
                // game_data["Scoreboard"] = game_scores[i]["name"] + game_scores[i]["score"];
            }
            // Print the game information
            cout << "Game Information for Game " << i + 1 << ":" << endl;
            for (auto it = game_data.begin(); it != game_data.end(); it++)
            {
                cout << it->first << ": " << it->second << endl;
            }
            for(const auto &over : isGame_over){
                over ? game_over = "Game over" : game_over = "Ongoing/Hasn't started";
                cout<<"Completed: "<<game_over<<endl;
            }
            cout << "Scoreboard: " << endl;
            //fix homescore and awayscore to handle games with no current score. 
            int homeScore, awayScore;
            for (auto &board : scoreboard)
            {
                if(board.first == game_data["Home Team"]){
                    homeScore = stoi(board.second);
                    cout<<board.first<<" : "<<homeScore<<endl;
                }
                else if(board.first == game_data["Away Team"]){
                    awayScore = stoi(board.second);
                    cout<<board.first<<" : "<<awayScore<<endl;
                }
            }
            
            cout << endl;

            //Fix table keys
            pqxx::connection conn("dbname = sports user = postgres");
            if(conn.is_open()){
                cout<<"Database is open"<<endl;
                pqxx::work s_table(conn);
                string insertQuery = "INSERT INTO scores(eventID, leagueName, home_team, away_team, home_score, away_score, completed, gametime) VALUES("
                + s_table.quote(game_data["ID(event)"]) + ", "
                +s_table.quote(game_data["League"]) + ", "
                +s_table.quote(game_data["Home Team"]) + ", "
                +s_table.quote(game_data["Away Team"]) + ", "
                //fix scoreboard, and completed game.
                +s_table.quote(homeScore) + ", "
                +s_table.quote(awayScore) + ", "
                +s_table.quote(game_over) + ", "
                +s_table.quote(game_data["Start Time"]) +  ")";
                s_table.exec(insertQuery);
                s_table.commit();
                conn.disconnect();
            }
            else{
                cerr<<"Could not insert data"<<endl;
            }
        }
    }
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