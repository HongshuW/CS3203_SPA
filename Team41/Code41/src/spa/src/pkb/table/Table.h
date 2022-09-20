//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <list>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class Table {
public:
    vector<string> header;
    vector<vector<string>> rows;

    // getters
    vector<string> getColumnByName(string columnName);
    vector<string> getRowByPrimaryKey(string key);
    shared_ptr<unordered_set<string>> getValuesByKey(string key, int keyColumnNumber = 0, int valueColumnNumber = 1);
    bool hasCol(string colName);

    // setters
    void renameHeader(vector<string> newHeader);
    void appendRow(vector<string> row);
    void appendRows(list<vector<string>> rows);
    void addValues(list<string> values);

    bool isEqual(Table otherTable);
    bool isHeaderEmpty();
    bool isBodyEmpty();

private:
    vector<string> getColumnByIndex(int index);

};

#endif //SPA_TABLE_H
