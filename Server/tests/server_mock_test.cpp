#include "../data/option_url.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::StartsWith;

TEST(ServerMockTest, ReturnCorrectSportsURL) {
    // Returns Correct API key.
    const string apiKey = std::getenv("SPORTS_ODDS_API_KEY");
    EXPECT_THAT(apiKey, StartsWith("6de"));
    
    std::string expected_url = "https://odds.p.rapidapi.com/v4/sports";
    string terminator = "\0";
    expected_url += "?all=false"+ terminator;

    OptionURL sport_option;
    OptionURL::RequestData requestData = sport_option.getRequestData("Sports", "", "sports", "");
    std::string actual_url = requestData.option_url;
    EXPECT_EQ(actual_url, expected_url);
}

TEST(ServerMockTest, ReturnsCorrectSportsScoreParamURL) {
    const string apiKey = std::getenv("SPORTS_ODDS_API_KEY");
    EXPECT_THAT(apiKey, StartsWith("6de"));
    
    std::string expected_url = "https://odds.p.rapidapi.com/v4/sports";
    string terminator = "\0";
    std::string sports_map_value = "baseball_mlb";
    expected_url += "/" + sports_map_value + "/scores" + terminator;

    OptionURL scores_option;
    std::string sport_req = "mlb";
    OptionURL::RequestData requestData = scores_option.getRequestData("Sports", sport_req, "scores", "");
    std::string actual_url =requestData.option_url;
    EXPECT_EQ(actual_url, expected_url);
    EXPECT_NE(actual_url, "");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}