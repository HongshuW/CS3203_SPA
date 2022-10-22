//
// Created by Nafour on 6/9/22.
//

#ifndef SPA_TABLECOMPARATOR_H
#define SPA_TABLECOMPARATOR_H

#include "pkb/table/Table.h"

class TableComparator {
 public:
  bool isEqual(Table testResultTable, Table expected);
};

#endif  // SPA_TABLECOMPARATOR_H
