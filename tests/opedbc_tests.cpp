#include <gtest/gtest.h>
#include "dbcparser.h"
#include "log.hpp"

extern const char _resource_tesla_can_dbc[];
extern const size_t _resource_tesla_can_dbc_len;

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

struct OpenDBCTest : public ::testing::TestWithParam<std::string> {
    CANdb::DBCParser parser;
};

TEST_P(OpenDBCTest, parse_dbc_file) {
    auto dbc_file = GetParam();
    ASSERT_TRUE(parser.parse(dbc_file));
}

INSTANTIATE_TEST_CASE_P(TeslaDBC, OpenDBCTest,
                        ::testing::Values(std::string{ _resource_tesla_can_dbc, _resource_tesla_can_dbc_len }));
