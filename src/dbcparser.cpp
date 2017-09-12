#include "dbcparser.h"
#include "Resource.h"
#include "lambda_visitor.hpp"
#include "log.hpp"

#include <peglib.h>
#include <fstream>

#include <boost/algorithm/string/erase.hpp>

extern const char _resource_dbc_grammar_peg[];
extern const size_t _resource_dbc_grammar_peg_len;

using namespace CANdb;
using strings = std::vector<std::string>;

DBCParser::DBCParser() {}

bool DBCParser::parse(const std::string& data) noexcept {
    Resource dbc{_resource_dbc_grammar_peg, _resource_dbc_grammar_peg_len};

    peg::parser parser;

    parser.log = [](size_t l, size_t k, const std::string& s) {
        cdb_error("Parser log {}:{} {}", l, k, s);
    };

    if (!parser.load_grammar(dbc.data(), dbc.size())) {
        cdb_error("Unable to parse grammar");
        return false;
    }

    parser.enable_trace(
        [](const char* a, const char* k, long unsigned int,
           const peg::SemanticValues&, const peg::Context&,
           const peg::any&) { cdb_trace(" Parsing {} \"{}\"", a, k); });

    cdb_trace("Parsing data={}", data);

    strings phrases;
    strings idents;
    std::vector<std::uint32_t> numbers;
    using PhrasePair = std::pair<std::uint32_t, std::string>;
    std::vector<PhrasePair> phrasesPairs;

    parser["version"] = [this, &phrases](const peg::SemanticValues&) {
        if (phrases.empty()) {
            throw peg::parse_error("Version phrase not found");
        }
        can_database.version = phrases.at(0);
        phrases.clear();
    };

    parser["phrase"] = [&phrases](const peg::SemanticValues& sv) {
        auto s = sv.token();
        boost::algorithm::erase_all(s, "\"");
        phrases.push_back(s);
    };

    parser["symbols"] = [this, &idents](const peg::SemanticValues& sv) {
        can_database.symbols = idents;
        cdb_debug("Found symbol {}", sv.c_str());
        idents.clear();
    };

    parser["TOKEN"] = [&idents](const peg::SemanticValues& sv) {
        auto s = sv.token();
        boost::algorithm::erase_all(s, "\n");
        cdb_debug("Found token {}", s);
        idents.push_back(s);
    };

    parser["ecus"] = [&idents, this](const peg::SemanticValues& sv) {
        can_database.ecus = idents;
        cdb_debug("Found ecus ");
        idents.clear();
    };

    parser["number"] = [&numbers, this](const peg::SemanticValues& sv) {
        auto number = std::stoi(sv.token(), nullptr, 10);
        cdb_debug("Found number {}", number);
        numbers.push_back(number);
    };

    parser["number_phrase_pair"] = [&phrasesPairs, &numbers, &phrases,
                                    this](const peg::SemanticValues& sv) {
        cdb_debug("number phrase pair");
        phrasesPairs.push_back(std::make_pair(numbers.at(0), phrases.at(0)));
    };


    parser["val_entry"] = [this, phrasesPairs](const peg::SemanticValues&) {
        std::vector<CANdb_t::ValTable::ValTableEntry> tab;
        cdb_debug("Val table entry found");
        std::transform(
            phrasesPairs.begin(), phrasesPairs.end(), std::back_inserter(tab),
            [](const auto& p) {
                return CANdb_t::ValTable::ValTableEntry{p.first, p.second};
            });
        can_database.val_tables.push_back(CANdb_t::ValTable{"", tab});
    };

    return parser.parse(data.c_str());
}

CANdb_t DBCParser::getDb() const { return can_database; }
