#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <string>
#include <nlohmann/json.hpp>
#include "curl_request.hpp"
#include "sql_queries.hpp"
using namespace std;
using namespace nlohmann;
json getSports(const char *aSports);
json getEventOdds(const char* event_odds_url);
json getScores(const char * scores_url);