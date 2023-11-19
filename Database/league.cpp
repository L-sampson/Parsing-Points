#include <iostream>
#include <pqxx/pqxx>
using namespace std;


int main (){
    const char * sql;
    pqxx::connection conn("dbname = sports user = postgres");
    if(conn.is_open()){
        cout<<"Opened databse!!"<<endl;
    }
    else{
        cout<<"Cannot open"<<endl;
    }

    sql = "INSERT INTO players(PersonID, FirstName, City)"
        "VALUES(1, 'Lavon', 'Decatur')";
    
    pqxx::work write(conn);
    write.exec(sql);
    write.commit();
    cout<<"Data added!";
    conn.disconnect();
    return 0; 
}