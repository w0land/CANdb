#include <gtest/gtest.h>
#include <iterator>

#include "dbcparser.h"
#include "log.hpp"

using strings = std::vector<std::string>;
std::shared_ptr<spdlog::logger> kDefaultLogger =
    []() -> std::shared_ptr<spdlog::logger> {
    auto z = std::getenv("CDB_LEVEL");
    auto logger = spdlog::stdout_color_mt("cdb");

    if (z == nullptr) {
        logger->set_level(spdlog::level::err);
    } else {
        const std::string ll{z};

        auto it = std::find_if(
            std::begin(spdlog::level::level_names),
            std::end(spdlog::level::level_names),
            [&ll](const char* name) { return std::string{name} == ll; });

        if (it != std::end(spdlog::level::level_names)) {
            int i = std::distance(std::begin(spdlog::level::level_names), it);
            logger->set_level(static_cast<spdlog::level::level_enum>(i));
        }
    }

    return logger;
}();

struct DBCParserTests : public ::testing::Test {
    CANdb::DBCParser parser;
};

struct SymbolsTest : public ::testing::TestWithParam<strings> {
    CANdb::DBCParser parser;
};

struct EcusTest : public ::testing::TestWithParam<strings> {
    CANdb::DBCParser parser;
};

struct ValuesTest : public ::testing::TestWithParam<strings> {
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
    EXPECT_TRUE(parser.parse("VERSION \"123 aa\" \n\n\n\n"));
    EXPECT_TRUE(parser.getDb().messages.empty());
    EXPECT_EQ(parser.getDb().version, "123 aa");
}

TEST_P(SymbolsTest, one_symbol) {
    auto params = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :

)";

    for (const auto& param : params) {
        dbc += "  ";
        dbc += param;
        dbc += "\n";
    }

    ASSERT_TRUE(parser.parse(dbc));
    EXPECT_EQ(parser.getDb().symbols, params);
}

TEST_P(EcusTest, single_line) {
    auto params = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
  NS_DESC
  NS_DESC2

BU_ :)";
    for (const auto& ecu : params) {
        dbc += " " + ecu;
    }
    dbc += "\n";
    ASSERT_TRUE(parser.parse(dbc));
    EXPECT_EQ(parser.getDb().ecus, params);
}

TEST_P(EcusTest, multi_line) {
    auto params = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
  NS_DESC
  NS_DESC2

BU_ :)";
    for (const auto& ecu : params) {
        dbc += std::string{"\n"} + std::string{"  "} + ecu;
    }
    dbc += "\n";
    ASSERT_TRUE(parser.parse(dbc));
    EXPECT_EQ(parser.getDb().ecus, params);
}

TEST_P(ValuesTest, val_table) {
    auto values = GetParam();
    std::string dbc =
        R"(VERSION ""
NS_ :
  NS_DESC
  NS_DESC2

BU_ :
  NEO
  MCU
  GTW

)";
    for (const auto& value : values) {
        dbc += std::string{"VAL_TABLE_ "} + value;
        dbc += "\n";
    }
    ASSERT_TRUE(parser.parse(dbc));

    EXPECT_EQ(parser.getDb().val_tables.size(), values.size());
}

// test case instantiations

INSTANTIATE_TEST_CASE_P(Ecus, EcusTest,
                        ::testing::Values(strings{"NEO"},
                                          strings{"NEO", "MCU"}));

INSTANTIATE_TEST_CASE_P(Symbols, SymbolsTest,
                        ::testing::Values(strings{"NS_DESC"},
                                          strings{"NS_DESC2", "NS_DESC"}));

INSTANTIATE_TEST_CASE_P(Values, ValuesTest,
                        ::testing::Values(strings{
                            "StW_AnglHP_Spd 16383 \"SNA\" ;",
                            "DI_aebFaultReason 15 "
                            "\"DI_AEB_FAULT_DAS_REQ_DI_UNAVAIL\" 14 "
                            "\"DI_AEB_FAULT_ACCEL_REQ_INVALID\" ;"}));
