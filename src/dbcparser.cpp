#include "dbcparser.h"
#include "log.hpp"

#include <fstream>

#include <boost/spirit/home/x3.hpp>

using namespace CANdb;

namespace {
std::vector<std::string> split_string(const std::string& data) {
    std::vector<std::string> ret;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = data.find('\n', prev)) != std::string::npos) {
        ret.push_back(data.substr(prev, pos - prev));
        prev = pos + 1;
    }

    ret.push_back(data.substr(prev));

    return ret;
}
}

DBCParser::DBCParser() {}

bool DBCParser::parse(const std::string& data) noexcept {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    if (data.empty()) return false;

    const auto split = split_string(data);

    if (split.empty()) {
        return false;
    }

    auto v = split.at(0);

    using x3::double_;
    using x3::lit;
    using x3::lexeme;
    using x3::_attr;
    using ascii::space;
    using ascii::char_;

    auto const quoted_string = lexeme['"' >> *(char_ - '"') >> '"'];
    std::string ver;
    auto version_cb = [this](const auto& v_) { can_database.version = _attr(v_); };
    x3::parse(v.begin(), v.end(),
                       // begin grammar
                       lit("VERSION") >> space >> quoted_string[version_cb]
                       // end grrammar
                       );


    return true;
}

CANdb_t DBCParser::getDb() const { return can_database; }
