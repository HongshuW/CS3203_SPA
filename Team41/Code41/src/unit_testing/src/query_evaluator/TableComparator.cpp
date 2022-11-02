//
// Created by Nafour on 6/9/22.
//

#include "TableComparator.h"

bool TableComparator::isEqual(Table testResultTable, Table expected) {
  if (testResultTable.getNumberOfColumns() != expected.getNumberOfColumns()) return false;
  if (testResultTable.getNumberOfRows() != expected.getNumberOfRows()) return false;
  for (int i = 0; i < expected.getNumberOfColumns(); ++i) {
    if (expected.getHeader()[i] != testResultTable.getHeader()[i]) return false;
  }
  for (int i = 0; i < expected.getNumberOfRows(); ++i) {
    auto expectedRow = expected.getRows()[i];
    auto testResultRow = testResultTable.getRows()[i];
    if (expectedRow.size() != testResultRow.size()) return false;
    for (int j = 0; j < expectedRow.size(); ++j) {
      if (expectedRow[j] != testResultRow[j]) return false;
    }
  }
  return true;
}
