#include "sql_queries.hpp"

using namespace std;
void insertLeagueData(map<string, string> &sports, vector<bool> active, string status){
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

void insertScores(map<string, string> game_data, vector<bool> isGame_over, string game_over, vector<pair<string, string>> scoreboard, int homeScore, int awayScore){
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

void insertOdds(map<string, string> game_odds,
        string book,
        string favorite,
        string underdog,
        string draw,
        double favoritePrice,
        double underdogPrice){
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