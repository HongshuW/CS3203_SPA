//
// Created by Nafour on 8/9/22.
//

#include "DummyDataRetriever2.h"

/*
 * procedure p2 {
 *  1  read x;
 *  2  read y;
 *  3  while ( y > 0 ) {
 *  4       x = x + 1
 *  5       if ( x > 2 ) {
 *  6            x = x + 2
 *          }
 *  7       y = y - 1
 *    }
 *  8  print x
 * }
 */

Table DummyDataRetriever2::getTableByDesignEntity(DesignEntity designEntity) {
    switch (designEntity) {
        case DesignEntity::PROCEDURE: {
            auto procedureTable = ProcedureTable();
            procedureTable.appendRow({"p2"});
            return procedureTable;
        }
        case DesignEntity::STMT: {
            auto stmtTable = StatementTable();
            stmtTable.appendRow({"1", getDesignEntityString(QB::DesignEntity::READ)});
            stmtTable.appendRow({"2", getDesignEntityString(QB::DesignEntity::READ)});
            stmtTable.appendRow({"3", getDesignEntityString(QB::DesignEntity::WHILE)});
            stmtTable.appendRow({"4", getDesignEntityString(QB::DesignEntity::ASSIGN)});
            stmtTable.appendRow({"5", getDesignEntityString(QB::DesignEntity::IF)});
            stmtTable.appendRow({"6", getDesignEntityString(QB::DesignEntity::ASSIGN)});
            stmtTable.appendRow({"7", getDesignEntityString(QB::DesignEntity::ASSIGN)});
            stmtTable.appendRow({"8", getDesignEntityString(QB::DesignEntity::PRINT)});

            return stmtTable;
        }
        case DesignEntity::VARIABLE: {
            auto varTable = VariableTable();
            varTable.appendRow({"x"});
            varTable.appendRow({"y"});

            return varTable;
        }

        case DesignEntity::CONSTANT:{
            auto constantTable = ConstantTable();
            constantTable.appendRow({"0"});
            constantTable.appendRow({"1"});
            constantTable.appendRow({"2"});

            return constantTable;
        }
        default:
            return Table();
    }
}

DesignEntity DummyDataRetriever2::getDesignEntityOfStmt(int stmtNumber) {
    auto de = getDesignEntity(this->getTableByDesignEntity(QB::DesignEntity::STMT).rows[stmtNumber-1][1]);
    return de;
}

DummyDataRetriever2::DummyDataRetriever2(shared_ptr<PKBStorage> pkbStorage) : DataRetriever(pkbStorage) {

}

Table DummyDataRetriever2::getFollowsTable() {
    auto table = FollowsTable();
    table.appendRow({"1", "2"});
    table.appendRow({"2", "3"});
    table.appendRow({"3", "8"});
    table.appendRow({"4", "5"});
    table.appendRow({"5", "7"});

    return table;
}

Table DummyDataRetriever2::getFollowsTTable() {
    auto table = FollowsTable();
    table.appendRow({"1", "2"});
    table.appendRow({"1", "3"});
    table.appendRow({"1", "8"});
    table.appendRow({"2", "3"});
    table.appendRow({"2", "8"});
    table.appendRow({"3", "8"});
    table.appendRow({"4", "5"});
    table.appendRow({"4", "7"});
    table.appendRow({"5", "7"});

    return table;
}

Table DummyDataRetriever2::getParentTable() {
    auto table = ParentTable();
    table.appendRow({"3", "4"});
    table.appendRow({"3", "5"});
    table.appendRow({"3", "7"});
    table.appendRow({"5", "6"});

    return table;
}

Table DummyDataRetriever2::getParentTTable() {
    auto table = ParentTable();
    table.appendRow({"3", "4"});
    table.appendRow({"3", "5"});
    table.appendRow({"3", "6"});
    table.appendRow({"3", "7"});
    table.appendRow({"5", "6"});
    return table;
}

Table DummyDataRetriever2::getUsesSTable() {
    auto table = UsesTable();
    table.appendRow({"3", "y"});
    table.appendRow({"3", "x"});
    table.appendRow({"4", "x"});
    table.appendRow({"5", "x"});
    table.appendRow({"6", "x"});
    table.appendRow({"7", "y"});
    table.appendRow({"8", "x"});
    return table;
}

Table DummyDataRetriever2::getModifiesSTable() {
    auto table = ModifiesTable();
    table.appendRow({"1", "x"});
    table.appendRow({"2", "y"});
    table.appendRow({"3", "x"});
    table.appendRow({"3", "y"});
    table.appendRow({"4", "x"});
    table.appendRow({"6", "x"});
    table.appendRow({"7", "y"});
    return table;
}

Table DummyDataRetriever2::getIfPatternTable() {
    auto table = PatternTable();
    table.appendRow({"5", "x"});
    return table;
}

Table DummyDataRetriever2::getWhilePatternTable() {
    auto table = PatternTable();
    table.appendRow({"3", "y"});
    return table;
}
