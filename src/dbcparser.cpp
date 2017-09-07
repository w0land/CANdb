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

namespace {}  // namespace

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

    std::string currPhrase = "junk";
    std::vector<std::string> idents;
    parser["version"] = [this, &currPhrase](const peg::SemanticValues&) {
        can_database.version = currPhrase;
    };

    parser["phrase"] = [&currPhrase](const peg::SemanticValues& sv) {
        auto s = sv.token();
        boost::algorithm::erase_all(s, "\"");
        currPhrase = s;
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

    return parser.parse(data.c_str());
}

CANdb_t DBCParser::getDb() const { return can_database; }
