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
    list<vector<string>> rows;

    list<string> getColumnByName(string columnName);

private:
    list<string> getColumnByIndex(int index);

};

#endif //SPA_TABLE_H
