#include <gtest/gtest.h>

#include <fstream>

#include "dbcparser.h"
#include "log.hpp"

extern const char _resource_tesla_can_dbc[];
extern const size_t _resource_tesla_can_dbc_len;

std::string loadDBCFile(const std::string& filename)
{
    const std::string path = std::string{ OPENDBC_DIR } + filename;

    std::fstream file{ path.c_str() };

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(buff));

    file.close();
    return buff;
}

std::shared_ptr<spdlog::logger> kDefaultLogger = []() -> std::shared_ptr<spdlog::logger> {
    auto z = std::getenv("CDB_LEVEL");
    auto logger = spdlog::stdout_color_mt("cdb");

    if (z == nullptr) {
        logger->set_level(spdlog::level::err);
    } else {
        const std::string ll{ z };

        auto it = std::find_if(std::begin(spdlog::level::level_names), std::end(spdlog::level::level_names),
            [&ll](const char* name) { return std::string{ name } == ll; });

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

TEST_P(OpenDBCTest, parse_dbc_file)
{
    auto dbc_file = GetParam();
    auto file = loadDBCFile(dbc_file);
    ASSERT_TRUE(parser.parse(file));
}

TEST_F(OpenDBCTest, quirks_ns)
{
    auto dbc_file = R"(VERSION ""


NS_ : 
	NS_DESC_
	CM_
	BA_DEF_
	BA_
	VAL_
	CAT_DEF_
	CAT_
	FILTER
	BA_DEF_DEF_
	EV_DATA_
	ENVVAR_DATA_
	SGTYPE_
	SGTYPE_VAL_
	BA_DEF_SGTYPE_
	BA_SGTYPE_
	SIG_TYPE_REF_
	VAL_TABLE_
	SIG_GROUP_
	SIG_VALTYPE_
	SIGTYPE_VALTYPE_
	BO_TX_BU_
	BA_DEF_REL_
	BA_REL_
	BA_DEF_DEF_REL_
	BU_SG_REL_
	BU_EV_REL_
	BU_BO_REL_
	SG_MUL_VAL_

BS_:

BO_ 271360000 GasPedalRegenCruise: 8 GMLAN
 SG_ CruiseControlActive : 56|1@0+ (1,0) [0|0] ""  GMLAN
 SG_ MaxRegen : 12|1@0+ (1,0) [0|1] ""  GMLAN,NEO
 SG_ GasPedal : 47|8@0+ (1,0) [0|254] ""  GMLAN,NEO
 SG_ GearShifter2NotUsed : 55|8@0+ (1,0) [0|255] ""  GMLAN,NEO

)";
    ASSERT_TRUE(parser.parse(dbc_file));
}

INSTANTIATE_TEST_CASE_P(TeslaDBC, OpenDBCTest,
    ::testing::Values("tesla_can.dbc", "acura_ilx_2016_can.dbc", "acura_ilx_2016_can.dbc", "acura_ilx_2016_nidec.dbc",
        "gm_global_a_chassis.dbc", "gm_global_a_lowspeed.dbc", "gm_global_a_object.dbc", "gm_global_a_powertrain.dbc",
        "honda_accord_touring_2016_can.dbc", "honda_civic_touring_2016_can.dbc", "honda_crv_ex_2017_can.dbc",
        "honda_crv_touring_2016_can.dbc", "subaru_outback_2016_eyesight.dbc", "tesla_can.dbc",
        "toyota_prius_2017_can0.dbc", "toyota_prius_2017_can1.dbc"));
