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
    EXPECT_TRUE(parser.parse("VERSION \"\" "));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "");
}

TEST_F(DBCParserTests, correct_version_number) {
    EXPECT_TRUE(parser.parse("VERSION \"123\" "));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "123");
}
