#include <gtest/gtest.h>
#include "dbcparser.h"

struct DBCParserTests : public ::testing::Test {
    CANdb::DBCParser parser;
};

TEST_F(DBCParserTests, empty_data) {
    EXPECT_FALSE(parser.parse(""));
    EXPECT_TRUE(parser.getDb().messages.empty());
}

TEST_F(DBCParserTests, one_liner) {
    EXPECT_TRUE(parser.parse("VERSION \"\""));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "");
}

TEST_F(DBCParserTests, correct_version_number) {
    EXPECT_TRUE(parser.parse("VERSION \"123\""));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "123");
}

TEST_F(DBCParserTests, network_mode) {
    const std::string dbc =
        "VERSION \"abd 1234\" \n \
        BU_: DummyNode";

    EXPECT_TRUE(parser.parse(dbc));
    EXPECT_TRUE(parser.getDb().messages.empty());
    ASSERT_FALSE(parser.getDb().nodes.empty());
}

TEST_F(DBCParserTests, simple_message_with_two_signals) {
    const std::string dbc =
        "BO_ 411 Info_CO_R: 8 IMC \n\
 SG_ TimedCharge1ActiveFri : 52|1@0+ (1,0) [0|1] \" \" Vector_XXX \n\
 SG_ TimedCharge1Active : 53|1@0+ (1,0) [0|1] \"\" Vector_XXX \n\
\
";

    EXPECT_TRUE(parser.parse(dbc));
}
