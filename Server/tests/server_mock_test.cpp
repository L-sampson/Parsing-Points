#include "../data/option_url.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::StartsWith;

TEST(ServerMockTest, ReturnCorrectSportsURL) {
    // Returns Correct API key.
    const string apiKey = std::getenv("SPORTS_ODDS_API_KEY");
    EXPECT_THAT(apiKey, StartsWith("fd4"));
    
    std::string expected_url = "https://api.the-odds-api.com/v4/sports/";
    string terminator = "\0";
    expected_url += "?apiKey=" + apiKey + terminator;

    OptionURL sports_option;
    const std::string option = "Sports";
    std::string actual_url = sports_option.getOptionUrl(option);
    EXPECT_EQ(actual_url, expected_url);
}

TEST(ServerMockTest, ReturnsCorrectSportsScoreParamURL) {
    const string apiKey = std::getenv("SPORTS_ODDS_API_KEY");
    EXPECT_THAT(apiKey, StartsWith("fd4"));
    
    std::string expected_url = "https://api.the-odds-api.com/v4/sports/";
    string terminator = "\0";
    std::string sports_map_value = "baseball_mlb";
    expected_url += sports_map_value + "/scores/?apiKey=" + apiKey + terminator;

    OptionURL sport_param;
    const std::string param = "mlb";
    std::string actual_url = sport_param.getSportsWithParamURL(param);
    EXPECT_EQ(actual_url, expected_url);
    EXPECT_NE(actual_url, "");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}