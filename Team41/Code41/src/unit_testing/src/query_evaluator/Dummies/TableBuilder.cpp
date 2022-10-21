//
// Created by Nafour on 6/9/22.
//

#include "TableBuilder.h"

#include <string>
#include <vector>

namespace TestQE {
TableBuilder* TableBuilder::setHeaders(vector<string> header) {
  this->header = header;
  return this;
}

TableBuilder* TableBuilder::addRow(vector<string> row) {
  this->rows.push_back(row);
  return this;
}

Table TableBuilder::build() {
  Table table = Table();
  table.header = this->header;
  for (auto row : this->rows) {
    table.rows.push_back(row);
  }
  return table;
}
}  // namespace TestQE