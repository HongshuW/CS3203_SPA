//
// Created by hongshu wang on 12/9/22.
//

#include "PatternTable.h"

PatternTable::PatternTable() {
    header = vector<string>{"$statement_number", "$statement_type", "$variable_name"};
}

bool PatternTable::isSubExpression(shared_ptr<ExprNode> pattern, shared_ptr<ExprNode> queriedPattern) {
    if (!pattern->left && !pattern->right) {
        return isSameExpression(pattern, queriedPattern);
    }
    return isSameExpression(pattern, queriedPattern) || isSubExpression(pattern->right, queriedPattern);
}

bool PatternTable::isSameExpression(shared_ptr<ExprNode> pattern, shared_ptr<ExprNode> queriedPattern) {
    return *pattern == *queriedPattern;
}

shared_ptr<Table> PatternTable::getMatchedPatterns(
        ExpressionSpecType expressionSpecType, shared_ptr<ExprNode> queriedPattern) {
    int size = patternColumn.size();
    Table outputTable;
    outputTable.header = vector<string>{"$statement_number", "$variable_name"};
    for (int i = 0; i < size; i++) {
        vector<string> metainfo = rows[i];
        string stmtNo = metainfo[0];
        string varName = metainfo[2];
        vector<string> row{stmtNo, varName};
        shared_ptr<ExprNode> pattern = patternColumn[i];
        switch (expressionSpecType) {
            case ExpressionSpecType::ANY_MATCH:
                outputTable.appendRow(row);
            case ExpressionSpecType::PARTIAL_MATCH:
                if (isSubExpression(pattern, queriedPattern)) {
                    outputTable.appendRow(row);
                }
            case ExpressionSpecType::FULL_MATCH:
                if (isSameExpression(pattern, queriedPattern)) {
                    outputTable.appendRow(row);
                }
        }
    }
    return make_shared<Table>(outputTable);
}