#include "get_bet.hpp"

void printBetSlip(vector<Bet> bets){
    ofstream final_bets("bets.txt", ios::trunc);
    if(final_bets.is_open()){
        double remainingDeposit;
        double totalWager =0;
        double totalP_earnings =0;
        // double remainingDeposit = bet.deposit;
        final_bets<<"Betting Slip\nGood Luck\n";
        final_bets<<"***************************************\n";
        for (const Bet &bet : bets)
            {
                final_bets << "Team: " << bet.team_choice << ", Wager: " << bet.wager << ", Predicted Earnings: " << bet.predicted_earnings << endl;
                remainingDeposit = bet.deposit;
                totalWager += bet.wager;
                totalP_earnings+=bet.predicted_earnings;
            }
            final_bets<<"Total Wager Amount: $"<<totalWager<<endl;
            final_bets<<"Total Predicted Earnings: $"<<totalP_earnings<<endl;
            final_bets << "Remaining Deposit: $" << remainingDeposit << endl;
            final_bets<<"Parsing *Points 🏈🏀🏒⚽🏏⛳🥊🥋\nFounded 2023\nDecatur, GA";
        final_bets.close();
    }
    else{
        cout<<"Unable to open the betting slip."<<endl;
    }
}

vector<Bet> userBet()
{
    double deposit = 0;
    cout<<"Current account balance: "<<deposit<<endl;
    cout<<"Please desposit money into your account to start waging bets: \n";
    cin>>deposit;
    if(deposit>0){
        cout<<"Great you deposited "<<deposit<<endl;
        cout<<"Let's start betting!\n";
    }
    else{
        cout<<"Invalid deposit amount. Please enter an amount greater than 0\n";
    }
    
    int gameNum;
    std::cout << "Choose a team to place a bet.\nSelect a game based on the game number!\n";
    std::cout << "Make you selection now: \n";
    cout<<"\n";

    // Create a vector to store bets
    vector<Bet> bets; 

    try
    {
        pqxx::connection bet("dbname = sports user = postgres");
        if (!bet.is_open())
        {
            cout << "Cannot open Database\n";
        }

        cout << "Avaiable game to bet on!\n";
        pqxx::work txn(bet);
        pqxx::result games = txn.exec("SELECT frontrunner, longshot, frprice, underprice FROM odds");
        char multiBets;

        do
        {
            
            for (int i = 0; i < games.size(); i++)
            {
                pqxx::row row(games[i]);
                auto frontrunner = row[0].as<string>();
                auto longshot = row[1].as<string>();
                auto favorite = row[2].as<double>();
                auto underdog = row[3].as<double>();
                cout << "Game: " << i + 1 << " " << frontrunner << "(" << favorite << ")"
                     << " vs. " << longshot << "(" << underdog << ")" << endl;
            }

            cout << "\nEnter the game number you want to place a wager on: ";

            cin >> gameNum;
            cin.get();

            if (gameNum <= 0 || gameNum > games.size())
            {
                std::cout << "Invalid game number. Please enter a valid game number: ";
            }

            // Call userWager to get bet details
            Bet newBet = userWager(games[gameNum - 1], deposit);
            // Add the bet to the vector 
            bets.push_back(newBet);
            deposit = newBet.deposit;                     
            for (const Bet &bet : bets)
            {
                std::cout << "Team: " << bet.team_choice << 
                ", Wager: " << bet.wager << 
                ", Predicted Earnings: " << bet.predicted_earnings<< 
                ", Remaining deposit: "<< bet.deposit<< endl;
            }

            cout << "Would you like to bet on another team?" << endl;
            cout << "Yes[y] to continue, press any other key to exit and view betting slip:";
            cin >> multiBets;

        } while (multiBets == 'y' || multiBets == 'Y');
        cout<<"\nPrinting Betting Slip\nThank you for using Parsing *Points!\n";
        printBetSlip(bets);
        txn.commit();
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << endl;
    }
    
    return bets; // Return the vector of bets
}

Bet userWager(pqxx::row row, double deposit)
{
    auto frontrunner = row[0].as<string>();
    auto longshot = row[1].as<string>();
    auto favorite = row[2].as<double>();
    auto underdog = row[3].as<double>();

    cout << "Which team would you like to bet on?\n";
    cout << frontrunner << " OR " << longshot << endl;

    string betTeam = "";
    double wager =0;
    double predicted_earnings;
    getline(cin, betTeam);
    while(true){
    cout<<"Account balance: "<<deposit<<endl;
    cout << "How much would you like to bet?\nEnter wager amount:\n";
    cin >> wager;
    cout<<"\n";

    if (betTeam == frontrunner)
    {
        predicted_earnings = (wager * (favorite - 1));
    }
    else if (betTeam == longshot)
    {
        predicted_earnings = (wager * (underdog - 1));
    }

    
        if(wager<=deposit){
            deposit-=wager;
            //cout<<"You have "<<deposit<<" left to bet on games\n";
            break;
        }
        else{
            cerr<<"Please enter a valid wager amount\n";
        }
    }

    Bet newBet;
    newBet.team_choice = betTeam;
    newBet.wager = wager;
    newBet.predicted_earnings = predicted_earnings;
    newBet.deposit = deposit;

    return newBet; // Return the bet object
}
