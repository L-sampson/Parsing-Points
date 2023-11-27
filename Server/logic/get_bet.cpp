#include "get_bet.hpp"

void userBet(){
    std::cout<<"Choose a team to place a bet.\n Select a game based on either the team name or game number!\n";
    std::cout<<"Make you selection now: \n";

    try{
        pqxx::connection bet("dbname = sports user = postgres");
        if(bet.is_open()){
            cout<<"Avaiable game to bet on!\n";
        pqxx::work txn(bet);
        pqxx::result games = txn.exec("SELECT * FROM odds");;
        for(int i = 0; i<games.size(); i++){
            pqxx::row row(games[i]);
            for(int j =0; j<row.size(); j++){
                cout<<row[j]<< " ";
            }
        }
        }
        else{
            cout<<"Cannot open Database\n";
        }
    }
    catch(const std::exception &e){
        cerr<<e.what()<<endl;
    }

    
}