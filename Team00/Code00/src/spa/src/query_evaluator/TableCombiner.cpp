//
// Created by Nafour on 6/9/22.
//

#include <unordered_set>
#include "TableCombiner.h"
#include "iostream"
using namespace std;
namespace QE {
    Table TableCombiner::crossProduct(const Table &t1, const Table &t2) {
        Table productTable = Table();
        productTable.header.insert(productTable.header.end(), t1.header.begin(), t1.header.end());
        productTable.header.insert(productTable.header.end(), t2.header.begin(), t2.header.end());
        for (auto row1: t1.rows) {
            for (auto row2: t2.rows) {
                auto productRow = vector<string>();
                productRow.insert(productRow.begin(), row2.begin(), row2.end());
                productRow.insert(productRow.begin(), row1.begin(), row1.end());
                productTable.rows.push_back(productRow);
            }
        }
        return productTable;
    }

    Table TableCombiner::joinTable(const Table &t1, const Table &t2) {
        int FIRST_DUP_IDX = 0;
        int SECOND_DUP_IDX = 1;

        Table productTable = this->crossProduct(t1, t2);
        Table joinResultTable = Table();
        auto dupHeaders = this->findDuplicateHeaders(t1.header, t2.header);
        unordered_set<int> colInxToRemove;
        for (auto dupPair: dupHeaders) {
            colInxToRemove.insert(dupPair[SECOND_DUP_IDX]);
        }

        for (int i = 0; i < productTable.header.size(); ++i) {
            if (colInxToRemove.count(i)) continue;
            joinResultTable.header.push_back(productTable.header[i]);
        }

        for (auto row: productTable.rows) {
            bool isAdding = true;
            for (auto dupHeaderPair: dupHeaders) {
                int col1Idx = dupHeaderPair[FIRST_DUP_IDX];
                int col2Idx = dupHeaderPair[SECOND_DUP_IDX];
                if (row[col1Idx] != row[col2Idx]) {
                    isAdding = false;
                }

            }
            if (isAdding) {
                vector<string> rowToInsert = vector<string>();
                for (int i = 0; i < row.size(); ++i) {
                    if (colInxToRemove.count(i)) continue;
                    rowToInsert.push_back(row[i]);
                }
                joinResultTable.rows.push_back(rowToInsert);
            }
        }
        return joinResultTable;
    }

    vector<vector<int>> TableCombiner::findDuplicateHeaders(vector<string> h1, vector<string> h2) {
        unordered_map<string, int> h1Map;
        vector<vector<int>> ans;
        int h1Offset = h1.size();
        for (int i = 0; i < h1.size(); ++i) {
            if (h1[i].find("$") != string::npos) continue; //skip default headers.
            h1Map.insert({h1[i], i});
        }
        for (int i = 0; i < h2.size(); ++i) {
            if (h1Map.find(h2[i]) != h1Map.end()) {
                //find dup
                ans.push_back(vector<int>{h1Map.at(h2[i]), i + h1Offset});
            }
        }
        return ans;
    }
} // QE