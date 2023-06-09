//
// Created by Nafour on 5/9/22.
//

#include "DummyDataRetriever.h"

#include "pkb/PKBStorage.h"
#include "pkb/table/UsesTable.h"
#include "query_evaluator/constants/ClauseVisitorConstants.h"
/*
 * procedure p1 {
 * 1 dummyVarA = 1;
 * 2 dummyVarB = dummyVarA;
 * 3 print dummyVarA
 * }
 */

DummyDataRetriever::DummyDataRetriever(shared_ptr<PKBStorage> pkbStorage)
    : DataRetriever(pkbStorage) {}

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
      for (const string& variable : variables) {
        rows.push_back(vector<string>{variable});
      }
      Table variableTable = Table(header, rows);
      return variableTable;
    }
    case QB::DesignEntity::STMT: {
      Table stmtTable = Table();
      stmtTable.renameHeader({PKBStorage::STATEMENT_TABLE_COL1_NAME,
                              PKBStorage::STATEMENT_TABLE_COL2_NAME});
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
  return relationTable;
}

Table DummyDataRetriever::getParentTable() { return Table(); }

Table DummyDataRetriever::getParentTTable() { return Table(); }

Table DummyDataRetriever::getUsesSTable() {
  Table relationTable = Table();
  relationTable.appendRow({"2", "dummyVarA"});
  relationTable.appendRow({"3", "dummyVarA"});
  return relationTable;
}

Table DummyDataRetriever::getUsesPTable() {
  Table relationTable = UsesTable();
  relationTable.appendRow({"p1", "dummyVarA"});
  relationTable.appendRow({"p1", "dummyVarB"});
  return relationTable;
}

Table DummyDataRetriever::getModifiesSTable() {
  Table relationTable = Table();
  relationTable.appendRow({"1", "dummyVarA"});
  relationTable.appendRow({"2", "dummyVarB"});
  return relationTable;
}

Table DummyDataRetriever::getModifiesPTable() {
  Table relationTable = ModifiesTable();
  relationTable.appendRow({"p1", "dummyVarA"});
  relationTable.appendRow({"p1", "dummyVarB"});
  return relationTable;
}

Table DummyDataRetriever::getCallsTable() { return Table(); }

Table DummyDataRetriever::getCallsTTable() { return Table(); }

Table DummyDataRetriever::getNextTable() {
  Table relationTable = NextTable();
  relationTable.appendRow({"1", "2"});
  relationTable.appendRow({"2", "3"});
  return relationTable;
}

Table DummyDataRetriever::getNextTTable() {
  Table relationTable = NextTable();
  relationTable.appendRow({"1", "2"});
  relationTable.appendRow({"2", "3"});
  relationTable.appendRow({"1", "3"});
  return relationTable;
}

Table DummyDataRetriever::getAffectsTable() {
  Table relationTable = AffectsTable();
  relationTable.appendRow({"1", "2"});
  return relationTable;
}

Table DummyDataRetriever::getAffectsTTable() {
  Table relationTable = AffectsTable();
  relationTable.appendRow({"1", "2"});
  return relationTable;
}

Table DummyDataRetriever::getAssignPatternTable(ExpressionSpec expressionSpec) {
  if (expressionSpec.expressionSpecType == QB::ExpressionSpecType::ANY_MATCH) {
    Table table = Table();
    table.renameHeader({PKBStorage::STATEMENT_TABLE_COL1_NAME,
                        PKBStorage::VARIABLE_TABLE_COL1_NAME});
    table.appendRow({"1", "dummyVarA"});
    table.appendRow({"2", "dummyVarB"});
    return table;
  }
  return DataRetriever::getAssignPatternTable(expressionSpec);
}

Table DummyDataRetriever::getNextTStatements(int stmtNo) {
  if (stmtNo == 1) {
    Table relationTable = NextTable();
    relationTable.appendRow({"1", "2"});
    relationTable.appendRow({"1", "3"});
    return relationTable;
  }
  if (stmtNo == 2) {
    Table relationTable = NextTable();
    relationTable.appendRow({"2", "3"});
    return relationTable;
  }
  if (stmtNo == 3) return NextTable();

  return NextTable();
}

Table DummyDataRetriever::getPreviousTStatements(int stmtNo) {
  if (stmtNo == 2) {
    Table relationTable = NextTable();
    relationTable.appendRow({"1", "2"});
    return relationTable;
  }
  if (stmtNo == 3) {
    Table relationTable = NextTable();
    relationTable.appendRow({"2", "3"});
    relationTable.appendRow({"1", "3"});
    return relationTable;
  }
  return NextTable();
}

Table DummyDataRetriever::getNextTResult(int precedingStatement,
                                         int ensuingStatement) {
  if ((precedingStatement == 1 && ensuingStatement == 2) ||
      (precedingStatement == 2 && ensuingStatement == 3) ||
      (precedingStatement == 1 && ensuingStatement == 1)) {
    return QE::ClauseVisitorConstants::TRUE_TABLE;
  }
  return QE::ClauseVisitorConstants::FALSE_TABLE;
}

Table DummyDataRetriever::getAffectedStatements(int stmtNo) {
  if (stmtNo == 1) {
    Table relationTable = AffectsTable();
    relationTable.appendRow({"1", "2"});
    return relationTable;
  }
  return AffectsTable();
}

Table DummyDataRetriever::getAffectingStatements(int stmtNo) {
  if (stmtNo == 2) {
    Table relationTable = AffectsTable();
    relationTable.appendRow({"1", "2"});
    return relationTable;
  }
  return AffectsTable();
}

Table DummyDataRetriever::getAffectsResult(int affectingStatement,
                                           int affectedStatement) {
  if ((affectingStatement == 1 && affectedStatement == 2)) {
    return QE::ClauseVisitorConstants::TRUE_TABLE;
  }
  return QE::ClauseVisitorConstants::FALSE_TABLE;
}

Table DummyDataRetriever::getAffectedTStatements(int stmtNo) {
  if (stmtNo == 1) {
    Table relationTable = AffectsTable();
    relationTable.appendRow({"1", "2"});
    return relationTable;
  }
  return AffectsTable();
}

Table DummyDataRetriever::getAffectingTStatements(int stmtNo) {
  if (stmtNo == 2) {
    Table relationTable = AffectsTable();
    relationTable.appendRow({"1", "2"});
    return relationTable;
  }
  return AffectsTable();
}

Table DummyDataRetriever::getAffectsTResult(int affectingStatement,
                                            int affectedStatement) {
  if ((affectingStatement == 1 && affectedStatement == 2)) {
    return QE::ClauseVisitorConstants::TRUE_TABLE;
  }
  return QE::ClauseVisitorConstants::FALSE_TABLE;
}
