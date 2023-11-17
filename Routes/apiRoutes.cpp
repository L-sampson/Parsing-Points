#include <iostream>
#include <curl/curl.h>
#include <string.h>
#include <nlohmann/json.hpp>
#include <vector>
#define base_url "https://api.the-odds-api.com/v4/sports/?apiKey=fd47356e9ba1963492996e83733444db"

using namespace std;
using namespace nlohmann;
// creating Api routes in the folder

void parse(string &readBuff){
    auto p = json::parse(readBuff);
    json pdata = p;
    cout<<pdata[0]["bookmakers"][0];
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
    CURLcode res;

    curl_easy_setopt(odds, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(odds,CURLOPT_URL, "https://api.the-odds-api.com/v4/sports/americanfootball_nfl/odds/?apiKey=fd47356e9ba1963492996e83733444db&regions=us&markets=h2h,spreads&oddsFormat=american");
    curl_easy_setopt(odds, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(odds, CURLOPT_WRITEDATA, &readOdds);
    cout<<"Success:\n";
    res = curl_easy_perform(odds);
    if(res != CURLE_OK){
        cerr<<"Error downloding website page: " <<curl_easy_strerror(res);
        curl_easy_cleanup(odds);
    }
    
    curl_easy_cleanup(odds);
    parse(readOdds);

}

//GET Scores

//GET Historical odds

//GET event odds



int main()
{
    string url = base_url;
    vector<char> getUrl(url.begin(), url.end());
    char buffer[getUrl.size() + 1];
    // Copy the vector contents to the buffer
    std::copy(getUrl.begin(), getUrl.end(), buffer);

    // Add the null terminator
    buffer[getUrl.size()] = '\0';

    //getSports(buffer);
    getOdds();
    
    
    // Pass the buffer to the function
    //getSports(buffer);
    // strlen(url);
    // strlen(api_key);
    // strcat(url, api_key);
    // url = url.append(api_key);
    // cout<<url<<endl;
    //getSports(getUrl);
}