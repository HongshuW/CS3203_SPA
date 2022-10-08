//
// Created by Nafour on 5/9/22.
//

#include "DummyDataRetriever.h"
#include "pkb/PKBStorage.h"
/*
 * procedure p1 {
 * 1 dummyVarA = 1;
 * 2 dummyVarB = dummyVarA;
 * 3 print dummyVarA
 * }
 */

//Table DummyDataRetriever::getTableByRelationType(RelationType relationType) {
//    Table relationTable = Table();
//    relationTable.renameHeader({"$col1", "$col2"});
//    switch (relationType) {
//        case QB::RelationType::FOLLOWS: {
//            relationTable.appendRow({"1", "2"});
//            relationTable.appendRow({"2", "3"});
//            break;
//        }
//        case QB::RelationType::FOLLOWS_T: {
//            relationTable.appendRow({"1", "2"});
//            relationTable.appendRow({"2", "3"});
//            relationTable.appendRow({"1", "3"});
//            break;
//        }
//        case QB::RelationType::USES_S: {
//            relationTable.appendRow({"2", "dummyVarA"});
//            relationTable.appendRow({"3", "dummyVarA"});
//            break;
//        }
//        case QB::RelationType::MODIFIES_S: {
//            relationTable.appendRow({"1", "dummyVarA"});
//            relationTable.appendRow({"2", "dummyVarB"});
//            break;
//        }
//        case QB::RelationType::PARENT: {
//            break;
//        }
//        case QB::RelationType::PARENT_T: {
//            break;
//
//        }
//            //todo: handle USES_P and MODIFIES_P
//        default: {
//            break;
//        }
//
//    }
//    return relationTable;
//}

DummyDataRetriever::DummyDataRetriever(shared_ptr<PKBStorage> pkbStorage) : DataRetriever(pkbStorage) {

}


DesignEntity DummyDataRetriever::getDesignEntityOfStmt(int line_no) {
    switch (line_no) {
        case 1: {
            return QB::DesignEntity::ASSIGN;
        }
        case 2: {
            return QB::DesignEntity::ASSIGN;
        }
        case 3: {
            return QB::DesignEntity::PRINT;
        }
    }
    return DataRetriever::getDesignEntityOfStmt(line_no);
}

Table DummyDataRetriever::getTableByDesignEntity(DesignEntity designEntity) {
    switch (designEntity) {
        case QB::DesignEntity::VARIABLE: {
            vector<string> header{"variable name"};
            vector<string> variables = vector<string>{"dummyVarA", "dummyVarB"};
            vector<vector<string>> rows;
            for (const string& variable: variables) {
                rows.push_back(vector<string>{variable});
            }
            Table variableTable;
            variableTable.rows = rows;
            variableTable.header = header;
            return variableTable;
        }
        case QB::DesignEntity::STMT: {
            Table stmtTable = Table();
            stmtTable.renameHeader({PKBStorage::STATEMENT_TABLE_COL1_NAME, PKBStorage::STATEMENT_TABLE_COL2_NAME});
            stmtTable.appendRow({"1", "assign"});
            stmtTable.appendRow({"2", "assign"});
            stmtTable.appendRow({"3", "print"});
            return stmtTable;
        }
        case QB::DesignEntity::CONSTANT: {
            ConstantTable constantTable = ConstantTable();
            constantTable.appendRow({"1"});
            return constantTable;
        }
        default: {
            break;
        }
    }
    return DataRetriever::getTableByDesignEntity(designEntity);
}

Table DummyDataRetriever::getFollowsTable() {
    Table relationTable = Table();
    relationTable.renameHeader({"$col1", "$col2"});
    relationTable.appendRow({"1", "2"});
    relationTable.appendRow({"2", "3"});
    return relationTable;
}

Table DummyDataRetriever::getFollowsTTable() {
    Table relationTable = Table();
    relationTable.appendRow({"1", "2"});
    relationTable.appendRow({"2", "3"});
    relationTable.appendRow({"1", "3"});
    return DataRetriever::getFollowsTTable();
}

Table DummyDataRetriever::getParentTable() {
    return Table();
}

Table DummyDataRetriever::getParentTTable() {
    return Table();
}

Table DummyDataRetriever::getUsesSTable() {
    Table relationTable = Table();
    relationTable.appendRow({"2", "dummyVarA"});
    relationTable.appendRow({"3", "dummyVarA"});
    return relationTable;
}

Table DummyDataRetriever::getUsesPTable() {
    return Table();
}

Table DummyDataRetriever::getModifiesSTable() {
    Table relationTable = Table();
    relationTable.appendRow({"1", "dummyVarA"});
    relationTable.appendRow({"2", "dummyVarB"});
    return DataRetriever::getModifiesSTable();
}

Table DummyDataRetriever::getModifiesPTable() {
    return Table();
}

Table DummyDataRetriever::getCallsTable() {
    return Table();
}

Table DummyDataRetriever::getCallsTTable() {
    return Table();
}

Table DummyDataRetriever::getNextTable() {
    return Table();
}

Table DummyDataRetriever::getNextTTable() {
    return Table();
}

Table DummyDataRetriever::getAffectsTable() {
    return Table();
}

Table DummyDataRetriever::getAffectsTTable() {
    return Table();
}

//Table DummyDataRetriever::getTableByExprPattern(ExpressionSpec expressionSpec) {
//    if (expressionSpec.expressionSpecType == QB::ExpressionSpecType::ANY_MATCH) {
//       Table table = Table();
//       table.renameHeader({PKBStorage::STATEMENT_TABLE_COL1_NAME, PKBStorage::VARIABLE_TABLE_COL1_NAME});
//       table.appendRow({"1", "dummyVarA"});
//       table.appendRow({"2", "dummyVarB"});
//       return table;
//    }
//    return DataRetriever::getTableByExprPattern(expressionSpec);
//}

