#include <iostream>
#include <pqxx/pqxx>
using namespace std;

class League {
public:
    League(int* id, string* title, string* sport, string* description)
        : id(*id), title(*title), sport(*sport), description(*description) {}

    int id;
    string title, sport, description;

  void add(pqxx::work& txn) {
    std::string sql = "INSERT INTO league (id, title, sport, description) VALUES (:id, :title, :sport, :description)";
    txn.exec(sql, ":id", id, ":title", title, ":sport", sport, ":description", description);
}
};

int main() {
    pqxx::connection conn("dbname = sports user = postgres");
    if (conn.is_open()) {
        std::cout << "Connected to Database." << std::endl;

        // Create a new League object
        int id = 10;
        string title = "NBA";
        string sport = "Basketball";
        string description = "National Basketball Association";

        League newLeague(&id, &title, &sport, &description);

        // Create a transaction
        pqxx::work txn(conn);

        // Add the new League object to the database
        newLeague.add(txn);

        // Commit the transaction
        txn.commit();

        // Read data from the league table
        pqxx::result res = txn.exec("SELECT * FROM league");

        for (const auto &row : res) {
            for (const auto &field : row) {
                std::cout << field.c_str() << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}