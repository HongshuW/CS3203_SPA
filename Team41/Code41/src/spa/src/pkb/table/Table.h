//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <list>
#include <string>
#include <vector>

using namespace std;

class Table {
public:
    vector<string> header;
    vector<vector<string>> rows;

    // getters
    vector<string> getColumnByName(string columnName);
    vector<string> getRowByPrimaryKey(string key);

    // setters
    void renameHeader(vector<string> newHeader);
    void appendRow(vector<string> row);

    bool isEqual(Table otherTable);

private:
    vector<string> getColumnByIndex(int index);

};

#endif //SPA_TABLE_H
