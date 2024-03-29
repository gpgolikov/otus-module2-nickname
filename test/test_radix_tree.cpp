#include <catch2/catch.hpp>

#include <vector>
#include <algorithm>
#include <iterator>
#include <locale>
#include <sstream>

#include <radix_tree.h>

#include "utils.h"

using namespace std;
using namespace griha;
using namespace Catch::Matchers;

TEST_CASE("insert", "[radix_tree]") {
    using radix_tree_type = radix_tree<char>;
    radix_tree_type rtree;
    using nicknames_type = vector<radix_tree_type::iterator::value_type>;
    nicknames_type nicknames;

    SECTION("alone value") {
        rtree.insert("aleksey");
        copy(rtree.begin(), rtree.end(), back_inserter(nicknames));
        REQUIRE_THAT(nicknames, Equals(nicknames_type {
            { "aleksey", "a" }
        }));
    }

    SECTION("different prefix for all") {
        rtree.insert("aleksey");
        rtree.insert("sasha");
        rtree.insert("misha");
        copy(rtree.begin(), rtree.end(), back_inserter(nicknames));
        // radix_tree uses map for internal storage, because of it 
        // values always ordered
        REQUIRE_THAT(nicknames, Equals(nicknames_type {
            { "aleksey", "a" },
            { "misha", "m" },
            { "sasha", "s" }
        }));
    }

    SECTION("one branch of prefixes - start at the biggest") {
        rtree.insert("aleksey");
        rtree.insert("aleks");
        rtree.insert("alek");
        copy(rtree.begin(), rtree.end(), back_inserter(nicknames));
        REQUIRE_THAT(nicknames, Equals(nicknames_type {
            { "alek", "a" },
            { "aleks", "aleks" },
            { "aleksey", "alekse" }
        }));
    }

    SECTION("two branches with common prefix") {
        rtree.insert("aleksey");
        rtree.insert("aleks");
        rtree.insert("alek");
        rtree.insert("alesha");
        copy(rtree.begin(), rtree.end(), back_inserter(nicknames));
        REQUIRE_THAT(nicknames, Equals(nicknames_type {
            { "alek", "alek" },
            { "aleks", "aleks" },
            { "aleksey", "alekse" },
            { "alesha", "ales" }
        }));
    }

    SECTION("insert of more different values in arbitrary orders") {
        rtree.insert("alek");
        rtree.insert("aleksey");
        rtree.insert("alesha");
        rtree.insert("aleks");
        rtree.insert("maksim");
        rtree.insert("mike");
        rtree.insert("antony");
        rtree.insert("ann");
        rtree.insert("dmitry");
        rtree.insert("zahar");
        rtree.insert("zaratustra");
        rtree.insert("fridrich");
        copy(rtree.begin(), rtree.end(), back_inserter(nicknames));
        REQUIRE_THAT(nicknames, Equals(nicknames_type {
            { "alek", "alek" },
            { "aleks", "aleks" },
            { "aleksey", "alekse" },
            { "alesha", "ales" },
            { "ann", "ann" },
            { "antony", "ant" },
            { "dmitry", "d" },
            { "fridrich", "f" },
            { "maksim", "ma" },
            { "mike", "mi" },
            { "zahar", "zah" },
            { "zaratustra", "zar" }
        }));
    }
}

TEST_CASE("insert utf-8", "[radix_tree]") {
    if (setlocale(LC_ALL, "ru_RU.UTF-8") == nullptr &&
        setlocale(LC_ALL, "en_US.UTF-8") == nullptr &&
        setlocale(LC_ALL, "C.UTF-8") == nullptr) {
        setlocale(LC_ALL, "");
    }

    using radix_tree_type = radix_tree<wchar_t>;
    radix_tree_type rtree;
    using nicknames_type = vector<radix_tree_type::iterator::value_type>;
    nicknames_type nicknames;

    SECTION("insert utf-8") {
        rtree.insert(u8"алек");
        rtree.insert(u8"алексей");
        rtree.insert(u8"алеша");
        rtree.insert(u8"алекс");
        rtree.insert(u8"максим");
        rtree.insert(u8"майк");
        rtree.insert(u8"антон");
        rtree.insert(u8"анна");
        rtree.insert(u8"дмитрий");
        rtree.insert(u8"захар");
        rtree.insert(u8"заратустра");
        rtree.insert(u8"фридрих");
        copy(rtree.begin(), rtree.end(), back_inserter(nicknames));
        REQUIRE_THAT(nicknames, Equals(nicknames_type {
            { L"алек", L"алек" },
            { L"алекс", L"алекс" },
            { L"алексей", L"алексе" },
            { L"алеша", L"алеш" },
            { L"анна", L"анн" },
            { L"антон", L"ант" },
            { L"дмитрий", L"д" },
            { L"заратустра", L"зар" },
            { L"захар", L"зах" },
            { L"майк", L"май" },
            { L"максим", L"мак" },
            { L"фридрих", L"ф" }
        }));
    }
}

TEST_CASE("print", "[radix_tree]") {
    radix_tree<char> rtree;
    rtree.insert("alek");
    rtree.insert("aleksey");
    rtree.insert("alesha");
    rtree.insert("aleks");
    rtree.insert("maksim");
    rtree.insert("mike");

    ostringstream os;
    os << rtree;

    REQUIRE_THAT(os.str(), Equals(
        "+ ale\n"
        "| + k$\n"
        "| | + s$\n"
        "| |   + ey$\n"
        "| + sha$\n"
        "+ m\n"
        "  + aksim$\n"
        "  + ike$\n"
    ));
}