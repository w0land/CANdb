#ifndef CANTYPES_HPP_ML9DFK7A
#define CANTYPES_HPP_ML9DFK7A

#include <cstdint>
#include <map>
#include <string>
#include <vector>

enum class CANsignalType { Int, Float, String };

struct CANsignal {
    std::string name;
    uint8_t startBit;
    uint8_t len;
    CANsignalType type;
};

struct CANmessage {
    uint32_t id;
    std::string name;
};

struct CANdb_t {
    std::map<CANmessage, std::vector<CANsignal> > messages;
    std::string version;
    std::vector<std::string> nodes;
};

#endif /* end of include guard: CANTYPES_HPP_ML9DFK7A */
