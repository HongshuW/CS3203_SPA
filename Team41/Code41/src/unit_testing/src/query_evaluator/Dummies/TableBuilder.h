//
// Created by Nafour on 6/9/22.
//

#ifndef SPA_TABLEBUILDER_H
#define SPA_TABLEBUILDER_H
#include <vector>

#include "pkb/table/Table.h"

using namespace std;
namespace TestQE {

class TableBuilder {
  vector<string> header;
  vector<vector<string>> rows;

 public:
  TableBuilder* setHeaders(vector<string> header);
  TableBuilder* addRow(vector<string> row);
  Table build();
};

}  // namespace TestQE

#endif  // SPA_TABLEBUILDER_H
