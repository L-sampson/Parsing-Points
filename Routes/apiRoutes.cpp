#include<iostream>
#include<curl/curl.h>


using namespace std;
//creating Api routes in the folder
int main (){
    // string API_KEY = "fd47356e9ba1963492996e83733444db";
    // string url ="https://api.the-odds-api.com/v4/sports/?apiKey=";
    // //concatenate the string to add API_KEY.
    // string apiURL = url.append(API_KEY);
    
    CURL *sports = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(sports, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(sports, CURLOPT_URL, "https://api.the-odds-api.com/v4/sports/?apiKey=fd47356e9ba1963492996e83733444db");

    res = curl_easy_perform(sports);
    if(res != CURLE_OK){
        cout<<"Error downloading web page: "<<curl_easy_strerror(res)<<endl;
        curl_easy_cleanup(sports);
        return 1;
    }
    curl_easy_cleanup(sports);
    return 0;

}