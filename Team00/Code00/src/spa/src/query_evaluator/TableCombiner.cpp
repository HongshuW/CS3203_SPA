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
        Table productTable = this->crossProduct(t1, t2);
        Table joinTableResult = Table();
        auto dupHeaders = this->findDuplicateHeaders(t1.header, t2.header);
        for (int i = 0; i < productTable.header.size(); ++i) {
            for (auto dupPair: dupHeaders) {
                
            }

        }

        for(auto row: productTable.rows) {
            for (auto dupHeaderPair: dupHeaders) {

            }

        }



//        int t1ColIdx = -1;
//        int t2ColIdx = -1;
//        int joinColsCount = 0;
//        int countNeeded = 2;
//        for (int i = 0 ; i < productTable.header.size(); i++) {
//            if (productTable.header[i] == colName) {
//                joinColsCount++;
//                if (t1ColIdx == -1) {
//                    t1ColIdx = i;
//                } else {
//                    t2ColIdx = i;
//                }
//            }
//        }
//        if (t1ColIdx == -1 || t2ColIdx == -1 || joinColsCount != countNeeded) {
//            cout << "error when joining table" << endl;
//            cout << "t1ColIdx: "  + t1ColIdx << endl;
//            cout << "t2ColIdx: "  + t2ColIdx << endl;
//            cout << "joinColsCount: "  + joinColsCount << endl;
//            return Table();
//        }
//
//        Table joinResultTable = Table();
//
//        //add headers excluding duplicated headers
//        for (int i = 0; i < productTable.header.size(); ++i) {
//            if (i != t2ColIdx) {
//                joinResultTable.header.push_back(productTable.header[i]);
//            }
//        }
//
//        //add rows
//        for (auto row: productTable.rows) {
//            if (row[t1ColIdx] == row[t2ColIdx]) {
//                auto rowToInsert = vector<string>();
//                for (int i = 0; i < row.size(); ++i) {
//                    //excluding duplicate col
//                    if (i != t2ColIdx) {
//                        rowToInsert.push_back(row[i]);
//                    }
//                }
//                joinResultTable.rows.push_back(rowToInsert);
//            }
//        }
//
        return joinResultTable;
    }

    vector<vector<int>> TableCombiner::findDuplicateHeaders(vector<string> h1, vector<string> h2) {
        unordered_map<string, int> h1Map;
        vector<vector<int>> ans;
        int h1Offset = h1.size();
        for (int i = 0; i < h1.size(); ++i) {
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