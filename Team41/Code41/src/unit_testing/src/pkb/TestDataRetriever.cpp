//
// Created by hongshu wang on 1/9/22.
//

#include "catch.hpp"
#include "pkb/DataRetriever.h"

using namespace std;

TEST_CASE("Test Data Retriever") {
    SECTION("Get Variables in a Table") {
        DataRetriever dr;
        Table table = dr.getVariables();

        REQUIRE(table.header == (*EntityManager::getVariables()).header);
        REQUIRE(table.rows == (*EntityManager::getVariables()).rows);
    }
}