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

BU_: ADAS RADAR NEO XXX


BO_ 768 VEHICLE_STATE: 8 ADAS
 SG_ SET_ME_XF9 : 7|8@0+ (1,0) [0|255] "" Vector__XXX
 SG_ VEHICLE_SPEED : 15|8@0+ (1,0) [0|255] "kph" Vector__XXX

BO_ 769 VEHICLE_STATE2: 8 ADAS
 SG_ SET_ME_0F18510 : 7|28@0+ (1,0) [0|268435455] "" Vector__XXX
 SG_ SET_ME_25A0000 : 27|28@0+ (1,0) [0|268435455] "" Vector__XXX

BO_ 1024 XXX_100: 8 RADAR

BO_ 1040 XXX_101: 8 RADAR

BO_ 1041 XXX_102: 8 RADAR

BO_ 1042 XXX_103: 8 RADAR

BO_ 1043 XXX_104: 8 RADAR

BO_ 1044 XXX_105: 8 RADAR

BO_ 1045 XXX_106: 8 RADAR

BO_ 1046 XXX_107: 8 RADAR

BO_ 1047 XXX_108: 8 RADAR

BO_ 1056 XXX_109: 8 RADAR

BO_ 1057 XXX_110: 8 RADAR

BO_ 1058 XXX_111: 8 RADAR

BO_ 1059 XXX_112: 8 RADAR

BO_ 1060 XXX_113: 8 RADAR

BO_ 1072 TRACK_0: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1073 TRACK_1: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1074 TRACK_2: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1075 TRACK_3: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1076 TRACK_4: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1077 TRACK_5: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1078 TRACK_6: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1079 TRACK_7: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1080 TRACK_8: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1081 TRACK_9: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1088 TRACK_10: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1089 TRACK_11: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1090 TRACK_12: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1091 TRACK_13: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1092 TRACK_14: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1093 TRACK_15: 8 RADAR
 SG_ LONG_DIST : 7|12@0+ (0.0625,0) [0|255.5] "m"  NEO
 SG_ NEW_TRACK : 11|1@0+ (1,0) [0|1] ""  NEO
 SG_ LAT_DIST : 9|10@0- (0.0625,0) [0|63.5] "m"  NEO
 SG_ REL_SPEED : 31|12@0- (0.03125,0) [0|127.5] "m/s"  NEO

BO_ 1279 XXX_114: 8 RADAR

BO_ 1280 XXX_115: 8 RADAR

BO_ 1296 XXX_116: 8 RADAR

BO_ 1297 XXX_117: 8 RADAR

BO_TX_BU_ 768 : NEO,ADAS;
BO_TX_BU_ 769 : NEO,ADAS;




)";
    ASSERT_TRUE(parser.parse(dbc_file));
}

INSTANTIATE_TEST_CASE_P(TeslaDBC, OpenDBCTest,
    ::testing::Values("tesla_can.dbc", "acura_ilx_2016_can.dbc", "acura_ilx_2016_can.dbc", "acura_ilx_2016_nidec.dbc",
        "gm_global_a_chassis.dbc", "gm_global_a_lowspeed.dbc", "gm_global_a_object.dbc", "gm_global_a_powertrain.dbc",
        "honda_accord_touring_2016_can.dbc", "honda_civic_touring_2016_can.dbc", "honda_crv_ex_2017_can.dbc",
        "honda_crv_touring_2016_can.dbc", "subaru_outback_2016_eyesight.dbc", "tesla_can.dbc",
        "toyota_prius_2017_can0.dbc", "toyota_prius_2017_can1.dbc"));
