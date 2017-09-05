#include "dbcparser.h"
#include "lambda_visitor.hpp"
#include "log.hpp"

#include <fstream>

#include <boost/spirit/home/x3.hpp>
#include <boost/variant.hpp>

using namespace CANdb;

namespace {

bool starts_with(const std::string& str, const std::string& prefix) {
    auto trimmed = str;

    trimmed.erase(trimmed.begin(),
                  std::find_if(trimmed.begin(), trimmed.end(),
                               [](char c) { return !std::isspace(c); }));

    return trimmed.substr(0, prefix.size()) == prefix;
}

std::vector<std::string> split_dbc_into_chunks(const std::string& data) {
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

std::vector<std::string> getAllMessages(const std::string& data) {
    const auto split = split_dbc_into_chunks(data);

    std::vector<std::string> filtered;

    std::copy_if(split.begin(), split.end(), std::back_inserter(filtered),
                 [](const auto& s) {
                     return starts_with(s, "BO_") || starts_with(s, "SG_") ||
                            s == "";
                 });

    std::vector<std::string> ret;
    std::string currLines;
    for (auto line : filtered) {
        if (starts_with(line, "BO_") || starts_with(line, "SG_")) {
            currLines += line;
        } else if (line == "") {
            ret.push_back(currLines);
            currLines.clear();
        }
    }

    return ret;
}

struct VersionTag {
    std::string version;
};

using var = boost::variant<VersionTag>;

const std::map<std::string, std::function<var(const std::string&)>> parsers{
    // clang-format off
    {"VERSION", [](const std::string& data) {
         // clang-format on
         VersionTag vt;
         namespace x3 = boost::spirit::x3;
         namespace ascii = boost::spirit::x3::ascii;
         using x3::double_;
         using x3::lexeme;
         using x3::_attr;
         using ascii::space;
         using ascii::char_;

         auto const quoted_string = lexeme['"' >> *(char_ - '"') >> '"'];
         std::string ver;
         auto version_cb = [&vt](const auto& v_) { vt.version = _attr(v_); };
         x3::parse(data.begin(), data.end(),
                   // begin grammar
                   x3::lit("VERSION") >> space >> quoted_string[version_cb]
                   // end grrammar
                   );
         return vt;
     }},
    {"BU_",
     [](const auto& data) {
         VersionTag vt;
         return vt;
     }}
    // clang-format on
};
// clang-format on

}  // namespace

DBCParser::DBCParser() {}

bool DBCParser::parse(const std::string& data) noexcept {
    if (data.empty()) {
        return false;
    }

    const auto canMessages = getAllMessages(data);
    const auto split = split_dbc_into_chunks(data);

    if (split.empty()) {
        return false;
    }

    for (const auto& line : split) {
        auto result_variant = parsers.at("VERSION")(line);

        auto visitor = make_lambda_visitor<void>(
            [this](VersionTag tag) { can_database.version = tag.version; });
        boost::apply_visitor(visitor, result_variant);
    }

    return true;
}

CANdb_t DBCParser::getDb() const { return can_database; }
