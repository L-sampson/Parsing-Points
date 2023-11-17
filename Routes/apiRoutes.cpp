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
    cout<<pdata[0];
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// GET methods:
void getSports(char buff[])
{
    string readBuff;
    CURL *sports = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(sports, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(sports, CURLOPT_URL, buff);
    curl_easy_setopt(sports, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(sports, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(sports, CURLOPT_WRITEDATA, &readBuff);

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
    parse(readBuff);
}

int main()
{
    string url = base_url;
    vector<char> getUrl(url.begin(), url.end());
    char buffer[getUrl.size() + 1];
    // Copy the vector contents to the buffer
    std::copy(getUrl.begin(), getUrl.end(), buffer);

    // Add the null terminator
    buffer[getUrl.size()] = '\0';

    getSports(buffer);
    
    
    // Pass the buffer to the function
    // getSports(buffer);
    // strlen(url);
    // strlen(api_key);
    // strcat(url, api_key);
    // url = url.append(api_key);
    // cout<<url<<endl;
    //getSports(getUrl);
}