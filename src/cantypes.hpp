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

namespace std {
template<> struct less<CANmessage>
{
    bool operator() (const CANmessage& lhs, const CANmessage& rhs) const
    {
        return lhs.id < rhs.id;
    }

};
} // namespace std

struct CANdb_t {
    std::map<CANmessage, std::vector<CANsignal> > messages;
    std::string version;
    std::vector<std::string> nodes;
    std::vector<std::string> symbols;
    std::vector<std::string> ecus;
};


#endif /* end of include guard: CANTYPES_HPP_ML9DFK7A */
