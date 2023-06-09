//
// Created by hongshu wang on 7/10/22.
//

#include "catch.hpp"
#include "pkb/table/AffectsTable.h"
#include "pkb/table/NextTable.h"

TEST_CASE("Test Cachable") {
  SECTION("Test next* clear cache") {
    NextTable nextTTable = NextTable();
    nextTTable.appendRow({"1", "2"});
    nextTTable.appendRow({"2", "3"});
    nextTTable.appendRow({"1", "3"});
    nextTTable.clearCache();
    REQUIRE(nextTTable.areAllRelationsCached() == false);
    REQUIRE(nextTTable.getNumberOfRows() == 0);
  }

  SECTION("Test affects and affects* clear cache") {
    AffectsTable affectsTable = AffectsTable();
    affectsTable.appendRow({"1, 5"});
    affectsTable.appendRow({"2, 3"});
    affectsTable.appendRow({"3, 4"});
    affectsTable.clearCache();
    REQUIRE(affectsTable.areAllRelationsCached() == false);
    REQUIRE(affectsTable.getNumberOfRows() == 0);
  }
}
