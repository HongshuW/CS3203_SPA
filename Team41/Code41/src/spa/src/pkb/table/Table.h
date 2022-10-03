//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <list>
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

using namespace std;

class Table {
public:
    vector<string> header;
    vector<vector<string>> rows;

    // getters
    vector<string> getColumnByName(string columnName);
    vector<string> getColumnByIndex(int index);
    vector<string> getRowByPrimaryKey(string key);
    shared_ptr<unordered_set<string>> getValuesByKey(string key, int keyColumnNumber = 0, int valueColumnNumber = 1);
    int getColIdxByName(string colName);

    // setters
    void renameHeader(vector<string> newHeader);
    void appendRow(vector<string> row);
    void appendRows(list<vector<string>> rows);
    void addValues(list<string> values);

    Table dropCol(int colIdx);
    Table dupCol(int colIdx, string dupColName = "$dup_col");

    bool hasCol(string colName);
    bool isEqual(const Table &otherTable);
    bool isHeaderEmpty() const;
    bool isBodyEmpty() const;



};

#endif //SPA_TABLE_H
