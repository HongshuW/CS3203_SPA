//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <list>
#include <vector>

using namespace std;

template <typename... Types>
class Table {
public:
    vector<string> header;
    list<tuple<Types...>> rows;

    Table(vector<string> header, list<tuple<Types...>> rows);
    void insertRow(tuple<Types...> row);
};

#endif //SPA_TABLE_H
