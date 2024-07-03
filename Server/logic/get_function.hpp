#include <string>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include "curl_request.hpp"
#include "sql_queries.hpp"
using namespace std;
using namespace nlohmann;
void getSports(const char *aSports);
void getOdds(std::string url);
void getScores(const char * aScores);