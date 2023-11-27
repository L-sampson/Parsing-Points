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
    int wager;
    int predicted_earnings;
};

void userBet();

