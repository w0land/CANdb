#include <gtest/gtest.h>

#include "log.hpp"
#include "dbcparser.h"

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

struct TeslaDBCParts  : public ::testing::Test {
    CANdb::DBCParser parser;
};

TEST_F(TeslaDBCParts, version)
{
  auto dbc = R"(VERSION ""
)";
  EXPECT_TRUE(parser.parse(dbc));
}

TEST_F(TeslaDBCParts, ns)
{
    auto dbc = R"(VERSION ""
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

)";
    EXPECT_TRUE(parser.parse(dbc));
    EXPECT_EQ(parser.getDb().symbols.size(), 28);
}

TEST_F(TeslaDBCParts, bs)
{
    auto dbc = R"(VERSION ""
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

)";
    EXPECT_TRUE(parser.parse(dbc));
}

TEST_F(TeslaDBCParts, bu)
{
  auto dbc = R"(VERSION ""
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

BU_:
  NEO
    MCU
    GTW
    EPAS
    DI
    ESP
    SBW
    STW

)";
  EXPECT_TRUE(parser.parse(dbc));
}

TEST_F(TeslaDBCParts, val_tables)
{
    auto dbc = R"(VERSION ""
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

BU_:
  NEO
    MCU
    GTW
    EPAS
    DI
    ESP
    SBW
    STW

VAL_TABLE_ StW_AnglHP_Spd 16383 "SNA" ;
VAL_TABLE_ DI_aebFaultReason 15 "DI_AEB_FAULT_DAS_REQ_DI_UNAVAIL" 14 "DI_AEB_FAULT_ACCEL_REQ_INVALID" 13 "DI_AEB_FAULT_MIN_TIME_BTWN_EVENTS" 12 "DI_AEB_FAULT_ESP_MIA" 11 "DI_AEB_FAULT_ESP_FAULT" 10 "DI_AEB_FAULT_EPB_NOT_PARKED" 9 "DI_AEB_FAULT_ACCEL_OUT_OF_BOUNDS" 8 "DI_AEB_FAULT_PM_REQUEST" 7 "DI_AEB_FAULT_VEL_EST_ABNORMAL" 6 "DI_AEB_FAULT_DAS_SNA" 5 "DI_AEB_FAULT_DAS_CONTROL_MIA" 4 "DI_AEB_FAULT_SPEED_DELTA" 3 "DI_AEB_FAULT_EBR_FAULT" 2 "DI_AEB_FAULT_PM_MIA" 1 "DI_AEB_FAULT_EPB_MIA" 0 "DI_AEB_FAULT_NONE" ;
VAL_TABLE_ DI_aebLockState 3 "AEB_LOCK_STATE_SNA" 2 "AEB_LOCK_STATE_UNUSED" 1 "AEB_LOCK_STATE_UNLOCKED" 0 "AEB_LOCK_STATE_LOCKED" ;
VAL_TABLE_ DI_aebSmState 7 "DI_AEB_STATE_FAULT" 6 "DI_AEB_STATE_EXIT" 5 "DI_AEB_STATE_STANDSTILL" 4 "DI_AEB_STATE_STOPPING" 3 "DI_AEB_STATE_ENABLE" 2 "DI_AEB_STATE_ENABLE_INIT" 1 "DI_AEB_STATE_STANDBY" 0 "DI_AEB_STATE_UNAVAILABLE" ;
VAL_TABLE_ DI_aebState 7 "AEB_CAN_STATE_SNA" 4 "AEB_CAN_STATE_FAULT" 3 "AEB_CAN_STATE_STANDSTILL" 2 "AEB_CAN_STATE_ENABLED" 1 "AEB_CAN_STATE_STANDBY" 0 "AEB_CAN_STATE_UNAVAILABLE" ;
VAL_TABLE_ DI_epbInterfaceReady 1 "EPB_INTERFACE_READY" 0 "EPB_INTERFACE_NOT_READY" ;
VAL_TABLE_ DI_gear 7 "DI_GEAR_SNA" 4 "DI_GEAR_D" 3 "DI_GEAR_N" 2 "DI_GEAR_R" 1 "DI_GEAR_P" 0 "DI_GEAR_INVALID" ;
VAL_TABLE_ DI_gpoReason 8 "DI_GPO_NUMREASONS" 7 "DI_GPO_CAPACITOR_OVERTEMP" 6 "DI_GPO_NOT_ENOUGH_12V" 5 "DI_GPO_NO_BATTERY_POWER" 4 "DI_GPO_AMBIENT_OVERTEMP" 3 "DI_GPO_FLUID_DELTAT" 2 "DI_GPO_STATOR_OVERTEMP" 1 "DI_GPO_HEATSINK_OVERTEMP" 0 "DI_GPO_OUTLET_OVERTEMP" ;
VAL_TABLE_ DI_immobilizerCondition 1 "DI_IMM_CONDITION_LEARNED" 0 "DI_IMM_CONDITION_VIRGIN_SNA" ;
VAL_TABLE_ DI_immobilizerState 7 "DI_IMM_STATE_FAULT" 6 "DI_IMM_STATE_FAULTRETRY" 5 "DI_IMM_STATE_RESET" 4 "DI_IMM_STATE_LEARN" 3 "DI_IMM_STATE_DISARMED" 2 "DI_IMM_STATE_AUTHENTICATING" 1 "DI_IMM_STATE_REQUEST" 0 "DI_IMM_STATE_INIT_SNA" ;
VAL_TABLE_ DI_limpReason 24 "DI_LIMP_NUMREASONS" 23 "DI_LIMP_CAPACITOR_OVERTEMP" 22 "DI_LIMP_GTW_MIA" 21 "DI_LIMP_TRQCMD_VALIDITY_UNKNOWN" 20 "DI_LIMP_DI_MIA" 19 "DI_LIMP_CONFIG_MISMATCH" 18 "DI_LIMP_HEATSINK_TEMP" 17 "DI_LIMP_PMREQUEST" 16 "DI_LIMP_PMHEARTBEAT" 15 "DI_LIMP_TRQ_CROSS_CHECK" 14 "DI_LIMP_EXTERNAL_COMMAND" 13 "DI_LIMP_WRONG_CS_CALIBRATION" 12 "DI_LIMP_STATOR_TEMP" 11 "DI_LIMP_DELTAT_TOO_NEGATIVE" 10 "DI_LIMP_DELTAT_TOO_POSITIVE" 9 "DI_LIMP_AMBIENT_TEMP" 8 "DI_LIMP_OUTLET_TEMP" 7 "DI_LIMP_LOW_FLOW" 6 "DI_LIMP_BMS_MIA" 5 "DI_LIMP_12V_SUPPLY_UNDERVOLTAGE" 4 "DI_LIMP_NO_FLUID" 3 "DI_LIMP_NO_FUNC_HEATSINK_SENSOR" 2 "DI_LIMP_NO_FUNC_STATORT_SENSOR" 1 "DI_LIMP_BUSV_SENSOR_IRRATIONAL" 0 "DI_LIMP_PHASE_IMBALANCE" ;
VAL_TABLE_ DI_mode 2 "DI_MODE_DYNO" 1 "DI_MODE_DRIVE" 0 "DI_MODE_UNDEF" ;
VAL_TABLE_ DI_motorType 14 "DI_MOTOR_F2AE" 13 "DI_MOTOR_F2AD" 12 "DI_MOTOR_F2AC" 11 "DI_MOTOR_F2AB" 10 "DI_MOTOR_F1AC" 9 "DI_MOTOR_SSR1A" 8 "DI_MOTOR_F1A" 7 "DI_MOTOR_M7M6" 6 "DI_MOTOR_M8A" 5 "DI_MOTOR_M7M5" 4 "DI_MOTOR_M7M4" 3 "DI_MOTOR_M7M3" 2 "DI_MOTOR_ROADSTER_SPORT" 1 "DI_MOTOR_ROADSTER_BASE" 0 "DI_MOTOR_SNA" ;
VAL_TABLE_ DI_speedUnits 1 "DI_SPEED_KPH" 0 "DI_SPEED_MPH" ;
VAL_TABLE_ DI_state 4 "DI_STATE_ENABLE" 3 "DI_STATE_FAULT" 2 "DI_STATE_CLEAR_FAULT" 1 "DI_STATE_STANDBY" 0 "DI_STATE_PREAUTH" ;
VAL_TABLE_ DI_velocityEstimatorState 4 "VE_STATE_BACKUP_MOTOR" 3 "VE_STATE_BACKUP_WHEELS_B" 2 "VE_STATE_BACKUP_WHEELS_A" 1 "VE_STATE_WHEELS_NORMAL" 0 "VE_STATE_NOT_INITIALIZED" ;

)";
    EXPECT_TRUE(parser.parse(dbc));
}

TEST_F(TeslaDBCParts, messages)
{
  auto dbc = R"(VERSION ""


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

BU_:
  NEO
    MCU
    GTW
    EPAS
    DI
    ESP
    SBW
    STW

VAL_TABLE_ StW_AnglHP_Spd 16383 "SNA" ;
VAL_TABLE_ DI_aebFaultReason 15 "DI_AEB_FAULT_DAS_REQ_DI_UNAVAIL" 14 "DI_AEB_FAULT_ACCEL_REQ_INVALID" 13 "DI_AEB_FAULT_MIN_TIME_BTWN_EVENTS" 12 "DI_AEB_FAULT_ESP_MIA" 11 "DI_AEB_FAULT_ESP_FAULT" 10 "DI_AEB_FAULT_EPB_NOT_PARKED" 9 "DI_AEB_FAULT_ACCEL_OUT_OF_BOUNDS" 8 "DI_AEB_FAULT_PM_REQUEST" 7 "DI_AEB_FAULT_VEL_EST_ABNORMAL" 6 "DI_AEB_FAULT_DAS_SNA" 5 "DI_AEB_FAULT_DAS_CONTROL_MIA" 4 "DI_AEB_FAULT_SPEED_DELTA" 3 "DI_AEB_FAULT_EBR_FAULT" 2 "DI_AEB_FAULT_PM_MIA" 1 "DI_AEB_FAULT_EPB_MIA" 0 "DI_AEB_FAULT_NONE" ;
VAL_TABLE_ DI_aebLockState 3 "AEB_LOCK_STATE_SNA" 2 "AEB_LOCK_STATE_UNUSED" 1 "AEB_LOCK_STATE_UNLOCKED" 0 "AEB_LOCK_STATE_LOCKED" ;
VAL_TABLE_ DI_aebSmState 7 "DI_AEB_STATE_FAULT" 6 "DI_AEB_STATE_EXIT" 5 "DI_AEB_STATE_STANDSTILL" 4 "DI_AEB_STATE_STOPPING" 3 "DI_AEB_STATE_ENABLE" 2 "DI_AEB_STATE_ENABLE_INIT" 1 "DI_AEB_STATE_STANDBY" 0 "DI_AEB_STATE_UNAVAILABLE" ;
VAL_TABLE_ DI_aebState 7 "AEB_CAN_STATE_SNA" 4 "AEB_CAN_STATE_FAULT" 3 "AEB_CAN_STATE_STANDSTILL" 2 "AEB_CAN_STATE_ENABLED" 1 "AEB_CAN_STATE_STANDBY" 0 "AEB_CAN_STATE_UNAVAILABLE" ;
VAL_TABLE_ DI_epbInterfaceReady 1 "EPB_INTERFACE_READY" 0 "EPB_INTERFACE_NOT_READY" ;
VAL_TABLE_ DI_gear 7 "DI_GEAR_SNA" 4 "DI_GEAR_D" 3 "DI_GEAR_N" 2 "DI_GEAR_R" 1 "DI_GEAR_P" 0 "DI_GEAR_INVALID" ;
VAL_TABLE_ DI_gpoReason 8 "DI_GPO_NUMREASONS" 7 "DI_GPO_CAPACITOR_OVERTEMP" 6 "DI_GPO_NOT_ENOUGH_12V" 5 "DI_GPO_NO_BATTERY_POWER" 4 "DI_GPO_AMBIENT_OVERTEMP" 3 "DI_GPO_FLUID_DELTAT" 2 "DI_GPO_STATOR_OVERTEMP" 1 "DI_GPO_HEATSINK_OVERTEMP" 0 "DI_GPO_OUTLET_OVERTEMP" ;
VAL_TABLE_ DI_immobilizerCondition 1 "DI_IMM_CONDITION_LEARNED" 0 "DI_IMM_CONDITION_VIRGIN_SNA" ;
VAL_TABLE_ DI_immobilizerState 7 "DI_IMM_STATE_FAULT" 6 "DI_IMM_STATE_FAULTRETRY" 5 "DI_IMM_STATE_RESET" 4 "DI_IMM_STATE_LEARN" 3 "DI_IMM_STATE_DISARMED" 2 "DI_IMM_STATE_AUTHENTICATING" 1 "DI_IMM_STATE_REQUEST" 0 "DI_IMM_STATE_INIT_SNA" ;
VAL_TABLE_ DI_limpReason 24 "DI_LIMP_NUMREASONS" 23 "DI_LIMP_CAPACITOR_OVERTEMP" 22 "DI_LIMP_GTW_MIA" 21 "DI_LIMP_TRQCMD_VALIDITY_UNKNOWN" 20 "DI_LIMP_DI_MIA" 19 "DI_LIMP_CONFIG_MISMATCH" 18 "DI_LIMP_HEATSINK_TEMP" 17 "DI_LIMP_PMREQUEST" 16 "DI_LIMP_PMHEARTBEAT" 15 "DI_LIMP_TRQ_CROSS_CHECK" 14 "DI_LIMP_EXTERNAL_COMMAND" 13 "DI_LIMP_WRONG_CS_CALIBRATION" 12 "DI_LIMP_STATOR_TEMP" 11 "DI_LIMP_DELTAT_TOO_NEGATIVE" 10 "DI_LIMP_DELTAT_TOO_POSITIVE" 9 "DI_LIMP_AMBIENT_TEMP" 8 "DI_LIMP_OUTLET_TEMP" 7 "DI_LIMP_LOW_FLOW" 6 "DI_LIMP_BMS_MIA" 5 "DI_LIMP_12V_SUPPLY_UNDERVOLTAGE" 4 "DI_LIMP_NO_FLUID" 3 "DI_LIMP_NO_FUNC_HEATSINK_SENSOR" 2 "DI_LIMP_NO_FUNC_STATORT_SENSOR" 1 "DI_LIMP_BUSV_SENSOR_IRRATIONAL" 0 "DI_LIMP_PHASE_IMBALANCE" ;
VAL_TABLE_ DI_mode 2 "DI_MODE_DYNO" 1 "DI_MODE_DRIVE" 0 "DI_MODE_UNDEF" ;
VAL_TABLE_ DI_motorType 14 "DI_MOTOR_F2AE" 13 "DI_MOTOR_F2AD" 12 "DI_MOTOR_F2AC" 11 "DI_MOTOR_F2AB" 10 "DI_MOTOR_F1AC" 9 "DI_MOTOR_SSR1A" 8 "DI_MOTOR_F1A" 7 "DI_MOTOR_M7M6" 6 "DI_MOTOR_M8A" 5 "DI_MOTOR_M7M5" 4 "DI_MOTOR_M7M4" 3 "DI_MOTOR_M7M3" 2 "DI_MOTOR_ROADSTER_SPORT" 1 "DI_MOTOR_ROADSTER_BASE" 0 "DI_MOTOR_SNA" ;
VAL_TABLE_ DI_speedUnits 1 "DI_SPEED_KPH" 0 "DI_SPEED_MPH" ;
VAL_TABLE_ DI_state 4 "DI_STATE_ENABLE" 3 "DI_STATE_FAULT" 2 "DI_STATE_CLEAR_FAULT" 1 "DI_STATE_STANDBY" 0 "DI_STATE_PREAUTH" ;
VAL_TABLE_ DI_velocityEstimatorState 4 "VE_STATE_BACKUP_MOTOR" 3 "VE_STATE_BACKUP_WHEELS_B" 2 "VE_STATE_BACKUP_WHEELS_A" 1 "VE_STATE_WHEELS_NORMAL" 0 "VE_STATE_NOT_INITIALIZED" ;


BO_ 1160 DAS_steeringControl: 4 NEO
 SG_ DAS_steeringControlType : 23|2@0+ (1,0) [0|0] "" EPAS
 SG_ DAS_steeringControlChecksum : 31|8@0+ (1,0) [0|0] "" EPAS
 SG_ DAS_steeringControlCounter : 19|4@0+ (1,0) [0|0] "" EPAS
 SG_ DAS_steeringAngleRequest : 6|15@0+ (0.1,-1638.35) [-1638.35|1638.35] "deg" EPAS
 SG_ DAS_steeringHapticRequest : 7|1@0+ (1,0) [0|0] "" EPAS
 
BO_ 257 GTW_epasControl: 3 NEO
 SG_ GTW_epasControlChecksum : 16|8@1+ (1,0) [0|255] "" NEO
 SG_ GTW_epasControlCounter : 12|4@1+ (1,0) [0|15] "" NEO
 SG_ GTW_epasControlType : 8|2@1+ (1,0) [4|-1] "" NEO
 SG_ GTW_epasEmergencyOn : 0|1@1+ (1,0) [2|-1] "" NEO
 SG_ GTW_epasLDWEnabled : 11|1@1+ (1,0) [2|-1] "" NEO
 SG_ GTW_epasPowerMode : 1|4@1+ (1,0) [4|14] "" NEO
 SG_ GTW_epasTuneRequest : 5|3@1+ (1,0) [8|-1] "" NEO
BO_ 57 XXX_1: 3 XXX

BO_TX_BU_ 228 : NEO,ADAS;
BO_TX_BU_ 506 : NEO,ADAS;
BO_TX_BU_ 780 : NEO,ADAS;
BO_TX_BU_ 829 : NEO,ADAS;

CM_ SG_ 419 GEAR "10 = reverse, 11 = transition";
CM_ SG_ 490 LONG_ACCEL "wheel speed derivative, noisy and zero snapping";

VAL_ 69 WprSw6Posn 7 "SNA" 6 "STAGE2" 5 "STAGE1" 4 "INTERVAL4" 3 "INTERVAL3" 2 "INTERVAL2" 1 "INTERVAL1" 0 "OFF" ;VAL_ 257 GTW_epasControlType 0 "WITHOUT" 1 "WITH_ANGLE" 3 "WITH_BOTH" 2 "WITH_TORQUE" ;

)";
  EXPECT_TRUE(parser.parse(dbc));
}
