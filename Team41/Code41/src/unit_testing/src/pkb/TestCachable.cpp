//
// Created by hongshu wang on 7/10/22.
//

#include "catch.hpp"
#include "pkb/table/AffectsTable.h"
#include "pkb/table/NextTable.h"

TEST_CASE("Test Cachable") {
    SECTION("Test next* clear cache") {
        NextTable nextTTable = NextTable();
        nextTTable.appendRow({"1","2"});
        nextTTable.appendRow({"2","3"});
        nextTTable.appendRow({"1","3"});
        nextTTable.clearCache();
        REQUIRE(nextTTable.isQueried() == false);
        REQUIRE(nextTTable.rows.size() == 0);
    }

    SECTION("Test affects and affects* clear cache") {
        AffectsTable affectsTable = AffectsTable();
        affectsTable.appendRow({"1, 5"});
        affectsTable.appendRow({"2, 3"});
        affectsTable.appendRow({"3, 4"});
        affectsTable.clearCache();
        REQUIRE(affectsTable.isQueried() == false);
        REQUIRE(affectsTable.rows.size() == 0);
    }
}
