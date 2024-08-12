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
json getSports(const char *sports_url, const std::vector<std::string>& headersList);
json getEventOdds(const char* event_odds_url, const std::vector<std::string>& headersList);
json getScores(const char * scores_url, const std::vector<std::string>& headersList);