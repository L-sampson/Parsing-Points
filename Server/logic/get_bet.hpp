#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <pqxx/pqxx>
#include <fstream>

using namespace std;
struct Bet
{
    std::string team_choice;
    int game_number;
    double wager;
    double predicted_earnings;
    double deposit;
};
void printBetSlip(vector<Bet> bets);
vector<Bet> userBet();
Bet userWager(pqxx::row row, double deposit);

