#include "get_bet.hpp"

void userBet(){
    int gameNum;
    std::cout<<"Choose a team to place a bet.\n Select a game based on either the team name or game number!\n";
    std::cout<<"Make you selection now: \n";

    try{
        pqxx::connection bet("dbname = sports user = postgres");
        if(bet.is_open()){
            cout<<"Avaiable game to bet on!\n";
        pqxx::work txn(bet);
        pqxx::result games = txn.exec("SELECT frontrunner, longshot, frprice, underprice FROM odds");
        for(int i = 0; i<games.size(); i++){
            pqxx::row row(games[i]);
            auto frontrunner = row[0];
            auto longshot = row[1];
            auto favorite = row[2];
            auto underdog = row[3];
            cout<<"Game: "<< i+1<<" "<< frontrunner<<"("<<favorite<<")"<< " vs. "<<longshot<<"("<<underdog<<")"<<endl;
        }
        cout<<"\nEnter the game number you want to place a wager on: ";
        
        cin>>gameNum;
        cin.get();
        if (gameNum <= 0 || gameNum > games.size()) {
            std::cout << "Invalid game number. Please enter a valid game number: ";
          } 
        bool makeBets = true;
        do{
            string multiBets;
            userWager(games[gameNum -1]);
            cout<<"Would you like to bet on another team?"<<endl;
            cout<<"Yes to continue, press any other key to exit and view betting slip:";
    
    getline(cin,multiBets);
    cin.get();
    
    if(multiBets != "yes"){
        makeBets = false;
    }
        }
        while (makeBets);
        
        txn.commit();
        bet.disconnect();
        }
        else{
            cout<<"Cannot open Database\n";
        }
    }
    catch(const std::exception &e){
        cerr<<e.what()<<endl;
    }
}

void userWager(pqxx::row row){
    auto frontrunner = row[0].as<string>();
    auto longshot = row[1].as<string>();
    auto favorite = row[2].as<double>();
    auto underdog = row[3].as<double>();
    
    cout<<"Which team would you like to bet on?\n";
    cout<<frontrunner<< " OR "<<longshot<<endl;

    string betTeam = "";
    double wager;
    double predicted_earnings;
    getline(cin, betTeam);
    cout<<"How much would you like to bet?\nEnter wager amount:\n";
    cin>>wager;
    if(betTeam == frontrunner){
        predicted_earnings = (wager * (favorite -1));
    }
    else if(betTeam == longshot){
        predicted_earnings = (wager * (underdog -1));
    }
    cout<<"Great you would like to bet\nWager: "<<wager<<"\nOn this team: "<<betTeam<<endl;
    cout<<"The predicted earnings are: "<<predicted_earnings<<endl;

    vector<pair<string, double>> betSlip;
    double total = 0;
    pair<string, double> betPair = make_pair(betTeam, predicted_earnings);
    for(const auto &betPair: betSlip){
        cout<<"Team: "<< betPair.first<<"Potential Earnings: "<<betPair.second<<endl;
        total += betPair.second;
        cout<<total;
    }
    
    
}