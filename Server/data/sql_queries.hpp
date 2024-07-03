#include <iostream>
#include <pqxx/pqxx>

using namespace std;

void insertLeagueData(map<string, string> &leagues, vector<bool> active, string status);
void insertScores(map<string, string> game_data, vector<bool> isGame_over, string game_over, vector<pair<string, string>> scoreboard, int homeScore, int awayScore);
void insertOdds(map<string, string> game_odds,
        string book,
        string favorite,
        string underdog,
        string draw,
        double favoritePrice,
        double underdogPrice);
void refreshDB(const string & table);