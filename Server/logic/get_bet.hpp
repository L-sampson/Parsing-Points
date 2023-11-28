#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<pqxx/pqxx>

using namespace std;
struct Bet
{
    std::string team_choice;
    int game_number;
    double wager;
    double predicted_earnings;
};

vector<Bet> userBet();
Bet userWager(pqxx::row row);

