# Parsing *Points
Sports Scores/Betting backend application that retrieves the latest scores and odds of a games, league, or sporting event

* Have you ever wanted to see the live scores for your favorite sport/team but didn’t want to parse through multiple websites? 
* Wanted to find the best betting odds on a game/event regardless of the bookmaker? 
* Maybe you want to store previous bets to see how successful you are at sport betting.

Parsing *Points has all the features a sport fan needs to place bets and see live scores.

## Features
Retrieves all in-season(sports)
Retrieves live and completed scores for the past 3 days.
Retrieves betting odds from multiple bookmakers in table to reference odds/scores.
Allows users to place bets and give a prediction for potential earnings.
Allows users to save their bets/predictions as a file(BetSlip) to print and take to their favorite sporting event. 



## Installation & Necessary Libraries
The Parsing *Points utilizes various files, functions and libraries to return the Sports Data into a readable format for the user.
Please complete the following steps to successfully run the program.

Clone Git Repository in to desired directory.
[Git Clone](git clone link)

Obtain an API KEY from The Odds-API https://the-odds-api.com/

 Install the necessary Libraries:
 1. Libcurl library to make HTTP GET Requests from The Odds-API. https://curl.se/libcurl/c/libcurl.html
 2. Nlohmann/Json library to parse through JSON objects in c++. https://json.nlohmann.me/
 3. Postgresql pqxx library to connect and store parsed data into SQL tables. https://pqxx.org/libpqxx/
 


