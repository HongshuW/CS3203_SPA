//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <vector>

using namespace std;

template <typename... Types>
class Table {
public:
    Table(vector<string> header, vector<tuple<Types...>> rows);
    tuple<Types...> getRowByIndex(int index);
    void insertRow(tuple<Types...> row);

private:
    static vector<string> header;
    vector<tuple<Types...>> rows;
};

#endif //SPA_TABLE_H
