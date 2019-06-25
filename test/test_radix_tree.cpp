#include <catch2/catch.hpp>

#include <vector>
#include <algorithm>
#include <iterator>

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

    rtree.insert("aleksey");
    copy(rtree.begin(), rtree.end(), back_inserter(nicknames));
    REQUIRE_THAT(nicknames, Equals(nicknames_type {
        { "aleksey", "a" }
    }));
}