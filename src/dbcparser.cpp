#include "dbcparser.h"
#include "Resource.h"
#include "lambda_visitor.hpp"
#include "log.hpp"

#include <peglib.h>
#include <fstream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/split.hpp>

extern const char _resource_dbc_grammar_peg[];
extern const size_t _resource_dbc_grammar_peg_len;

template <typename T>
auto take_first(T& container) -> typename T::value_type {
    if (container.empty()) {
        throw std::runtime_error("container is empty");
    }
    auto v = container.front();
    container.pop_front();

    return v;
}
template <typename T>
auto take_back(T& container) -> typename T::value_type {
    if (container.empty()) {
        throw std::runtime_error("empty contaienr");
    }
    auto v = container.back();
    container.pop_back();

    return v;
}

template <typename T>
auto to_vector(const T& container) -> std::vector<typename T::value_type> {
    std::vector<typename T::value_type> ret;
    std::transform(std::begin(container), std::end(container),
                   std::back_inserter(ret), [](const auto& v) { return v; });
    return ret;
}

using namespace CANdb;
using strings = std::vector<std::string>;

std::string withLines(const std::string& dbcFile) {
    strings split;
    boost::split(split, dbcFile, boost::is_any_of("\n"));

    std::string buff;

    int counter{0};
    for (const auto& line : split) {
        buff += std::to_string(counter++) + std::string{": "} + line +
                std::string{"\n"};
    }

    return buff;
}

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

    cdb_debug("DBC file  = \n{}", withLines(data));

    strings phrases;
    std::deque<std::string> idents, signs;
    std::deque<std::int32_t> numbers;
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
        can_database.symbols = to_vector(idents);
        cdb_debug("Found symbols {}", sv.c_str());
        idents.clear();
    };

    parser["TOKEN"] = [&idents](const peg::SemanticValues& sv) {
        auto s = sv.token();
        boost::algorithm::erase_all(s, "\n");
        cdb_debug("Found token {}", s);
        idents.push_back(s);
    };

    parser["sign"] = [&signs](const peg::SemanticValues& sv) {
        cdb_debug("Found sign {}", sv.token());
        signs.push_back(sv.token());
    };

    parser["ecus"] = [&idents, this](const peg::SemanticValues& sv) {
        can_database.ecus = to_vector(idents);
        cdb_debug("Found ecus {}", sv.c_str());
        idents.clear();
    };

    parser["number"] = [&signs, &numbers, this](const peg::SemanticValues& sv) {
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

    std::vector<CANsignal> signals;
    parser["message"] = [this, &numbers, &signals,
                         &idents](const peg::SemanticValues& sv) {
        cdb_debug("Found a message {} signals = {}", idents.size(),
                  signals.size());
        if (numbers.size() < 2 || idents.size() < 2) {
            return;
        }
        CANmessage msg{static_cast<std::uint32_t>(numbers.at(0)), idents.at(0),
                       static_cast<std::uint32_t>(numbers.at(1)), idents.at(1)};
        cdb_debug("Found a message with id = {}", msg.id);
        can_database.messages[msg] = signals;
        signals.clear();
        numbers.clear();
        idents.clear();
    };

    parser["signal"] = [&idents, &numbers, &phrases, &signals,
                        &signs](const peg::SemanticValues& sv) {
        cdb_debug("Found signal {}", sv.c_str());

        auto receiver = take_back(idents);
        auto unit = take_back(phrases);

        auto max = take_back(numbers);
        auto min = take_back(numbers);
        auto offset = take_back(numbers);
        auto factor = take_back(numbers);

        auto value_type = take_back(signs);

        auto byteOrder = take_back(numbers);
        auto signalSize = take_back(numbers);
        auto startBit = take_back(numbers);

        auto signal_name = take_back(idents);

        signals.push_back(CANsignal{signal_name, startBit, signalSize,
                                    byteOrder, value_type, factor, offset, min,
                                    max, unit, receiver});
    };

    return parser.parse(data.c_str());
}

CANdb_t DBCParser::getDb() const { return can_database; }
