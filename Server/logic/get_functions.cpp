#include "get_function.hpp"
#include "get_bet.hpp"

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
            
            // insertLeagueData(sports, active, status);
            
        }
    }
    // refreshDB("leagues");
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
            insertOdds(game_odds, book,favorite,underdog,draw,favoritePrice,underdogPrice);
        }
        bool bet_games;
            string bet_option;
            cout<<"Would you like to bet on a game?\nEnter Yes or No:\n";
            getline(cin, bet_option);
            for(char s : bet_option){
                s = tolower(s);
            }
            if(bet_option == "yes"){
                bet_games;
                cout<<"Great let's place some bets!!!\n";
                userBet();
            }
            else{
                !bet_games;
                cout<<"Thank you for searching all the latest odds on Parsing *Points\n";
            }
    }
    refreshDB("odds");
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

            insertScores(game_data, isGame_over, game_over, scoreboard, homeScore, awayScore);
        }
    }
    refreshDB("scores");
}
/*Stretch Goals:
Get Historical Odds
Get Event Odds
refactor to make code more DRY!!!*/
