//
// Created by hongshu wang on 12/9/22.
//

#include "PatternTable.h"

#include "../PKBStorage.h"

PatternTable::PatternTable() {
  header = vector<string>{PKBStorage::STATEMENT_TABLE_COL1_NAME,
                          PKBStorage::VARIABLE_TABLE_COL1_NAME};
}

bool PatternTable::isSubExpression(shared_ptr<ExprNode> pattern,
                                   shared_ptr<ExprNode> queriedPattern) {
  if (pattern == nullptr || queriedPattern == nullptr) {
    return pattern == nullptr && queriedPattern == nullptr;
  }
  return isSameExpression(pattern, queriedPattern) ||
         isSubExpression(pattern->left, queriedPattern) ||
         isSubExpression(pattern->right, queriedPattern);
}

bool PatternTable::isSameExpression(shared_ptr<ExprNode> pattern,
                                    shared_ptr<ExprNode> queriedPattern) {
  if (pattern == nullptr || queriedPattern == nullptr) {
    return pattern == nullptr && queriedPattern == nullptr;
  }
  return pattern->expr == queriedPattern->expr &&
         isSameExpression(pattern->left, queriedPattern->left) &&
         isSameExpression(pattern->right, queriedPattern->right);
}

shared_ptr<Table> PatternTable::getMatchedPatterns(
    ExpressionSpec expressionSpec) {
  ExpressionSpecType expressionSpecType = expressionSpec.expressionSpecType;
  shared_ptr<ExprNode> queriedPattern = expressionSpec.exprNode;
  int size = patternColumn.size();
  Table outputTable = Table();
  outputTable.header = this->header;
  for (int i = 0; i < size; i++) {
    vector<string> metainfo = rows[i];
    shared_ptr<ExprNode> pattern = patternColumn[i];
    if (expressionSpecType == ExpressionSpecType::ANY_MATCH) {
      outputTable.appendRow(metainfo);
    } else if (expressionSpecType == ExpressionSpecType::PARTIAL_MATCH) {
      if (isSubExpression(pattern, queriedPattern)) {
        outputTable.appendRow(metainfo);
      }
    } else if (expressionSpecType == ExpressionSpecType::FULL_MATCH) {
      if (isSameExpression(pattern, queriedPattern)) {
        outputTable.appendRow(metainfo);
      }
    }
  }
  return make_shared<Table>(outputTable);
}

void PatternTable::addPattern(vector<string> metainfo,
                              shared_ptr<ExprNode> pattern) {
  appendRow(metainfo);
  patternColumn.push_back(pattern);
}
