#include <iostream>
#include <curl/curl.h>
#include<fstream>
#include <string.h>
#include<nlohmann/json.hpp>
#include <vector>
#define base_url "https://api.the-odds-api.com/v4/sports/?apiKey="

using namespace std;
using namespace nlohmann;
// creating Api routes in the folder

string envFile(){
    ifstream envFile("../apiKey.env");

    // Check if the file is open
    if (!envFile.is_open()) {
        return "Error opening .env file";
        
    }

    // Read the API key from the file
    string apiKey;
    getline(envFile, apiKey);
    string url = base_url;
    if (apiKey.empty()) {
        return "API key not found in .env file";
        
    }
    url = url.append(apiKey);
    envFile.close();
    return url;
}

void parse(string &readBuff){
    auto p = json::parse(readBuff);
    json pdata = p;
    for(int i =0; i<pdata.size(); i++){
        cout<<pdata[i]["title"]<<endl;
    }
}
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// GET sports methods
void getSports(char buff[])
{
    string readSports;
    CURL *sports = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(sports, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(sports, CURLOPT_URL, buff);
    curl_easy_setopt(sports, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(sports, CURLOPT_WRITEDATA, &readSports);

    res = curl_easy_perform(sports);
    if (res != CURLE_OK)
    {
        cout << "Error downloading web page: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(sports);
    }
    curl_easy_cleanup(sports);
    cout << "\nSuccess\n";
    // return readBuff;
    //fix
    parse(readSports);
}

//GET Odds Endpoint
void getOdds(){
    string readOdds;
    CURL *odds = curl_easy_init();
    CURLU * odds_handle;
    CURLcode res;

    if(odds){
        odds_handle = curl_url();
    }
    
    string sports ="https://api.the-odds-api.com/v4/sports/";
    string current = "/upcoming/odds/";
    string api = "?apiKey=fd47356e9ba1963492996e83733444db&regions=us&markets=h2h";
    sports +=current + api;
    //cout<<sports<<endl;
    //current = current.append(api);
    vector<char>sport(sports.begin(), sports.end());

    curl_url_set(odds_handle, CURLUPART_URL, sport.data(), 0);
    //curl_url_set(odds_handle, CURLUPART_PATH, sport, 0);
    //curl_easy_setopt(odds, CURLOPT_CUSTOMREQUEST, "GET");
    //curl_easy_setopt(odds,CURLOPT_CURLU, odds_handle);
    //curl_easy_setopt(odds, CURLOPT_WRITEFUNCTION, WriteCallback);
    //curl_easy_setopt(odds, CURLOPT_WRITEDATA, &readOdds);
    cout<<"Success:\n";
    res = curl_easy_perform(odds);
    if(res != CURLE_OK){
        cerr<<"Error downloding website page: " <<curl_easy_strerror(res);
        curl_easy_cleanup(odds);
    }
    
    curl_easy_cleanup(odds);
    curl_url_cleanup(odds_handle);
    //parse(readOdds);

}

//GET Scores

//GET Historical odds

//GET event odds



int main()
{
    string url = envFile();
    vector<char> getUrl(url.begin(), url.end());
    char buffer[getUrl.size() + 1];
    // Copy the vector contents to the buffer
    std::copy(getUrl.begin(), getUrl.end(), buffer);

    // Add the null terminator
    buffer[getUrl.size()] = '\0';

    getOdds();
    
    // Pass the buffer to the function
    //getSports(buffer);
//     string word_to_replace = "sports";
//     string word = "upcoming/odds";
//     size_t pos = url.find(word_to_replace);
//     if (pos != string::npos) {
//     url.replace(pos, word_to_replace.length(), word);
//   }
//     //string newUrl = url.replace(32,word.length(), word);
//     cout<<url<<endl;
    
}