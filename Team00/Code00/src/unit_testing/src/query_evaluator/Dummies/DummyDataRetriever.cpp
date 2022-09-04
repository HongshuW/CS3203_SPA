//
// Created by Nafour on 4/9/22.
//

#include "DummyDataRetriever.h"

namespace TestQE {
    Table DummyDataRetriever::getVariables() {
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
} // TestQE