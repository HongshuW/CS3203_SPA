//
// Created by hongshu wang on 12/9/22.
//

#include "PatternTable.h"

PatternTable::PatternTable() {
    header = vector<string>{"$statement_number", "$variable_name"};
}

bool PatternTable::isSubExpression(shared_ptr<ExprNode> pattern, shared_ptr<ExprNode> queriedPattern) {
    if (!pattern->left && !pattern->right) {
        return isSameExpression(pattern, queriedPattern);
    }
    return isSameExpression(pattern, queriedPattern)
        || isSubExpression(pattern->left, queriedPattern)
        || isSubExpression(pattern->right, queriedPattern);
}

bool PatternTable::isSameExpression(shared_ptr<ExprNode> pattern, shared_ptr<ExprNode> queriedPattern) {
    return *pattern == *queriedPattern;
}

shared_ptr<Table> PatternTable::getMatchedPatterns(ExpressionSpec expressionSpec) {
    ExpressionSpecType expressionSpecType = expressionSpec.expressionSpecType;
    shared_ptr<ExprNode> queriedPattern = expressionSpec.exprNode;
    int size = patternColumn.size();
    Table outputTable;
    outputTable.header = header;
    for (int i = 0; i < size; i++) {
        vector<string> metainfo = rows[i];
        shared_ptr<ExprNode> pattern = patternColumn[i];
        switch (expressionSpecType) {
            case ExpressionSpecType::ANY_MATCH:
                outputTable.appendRow(metainfo);
            case ExpressionSpecType::PARTIAL_MATCH:
                if (isSubExpression(pattern, queriedPattern)) {
                    outputTable.appendRow(metainfo);
                }
            case ExpressionSpecType::FULL_MATCH:
                if (isSameExpression(pattern, queriedPattern)) {
                    outputTable.appendRow(metainfo);
                }
        }
    }
    return make_shared<Table>(outputTable);
}

void PatternTable::addPattern(vector<string> metainfo, shared_ptr<ExprNode> pattern) {
    appendRow(metainfo);
    patternColumn.push_back(pattern);
}
