//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_SELECTCLAUSE_H
#define SPA_SELECTCLAUSE_H

#pragma once

#include <ostream>
#include <variant>
#include <vector>

#include "pkb/table/Table.h"
#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/commons/AttrRef.h"
#include "query_builder/commons/Synonym.h"
#include "query_evaluator/IVisitor.h"

using namespace std;

namespace QB {
enum class ReturnType { TUPLE, BOOLEAN };

using Elem = variant<Synonym, AttrRef>;
typedef shared_ptr<vector<Elem>> Tuple;

class SelectClause : public AbstractClause,
                     public enable_shared_from_this<SelectClause> {
 public:
  //! Can be Synonym, Tuple or BOOLEAN
  ReturnType returnType;
  //! To store results for single synonym and tuple only;
  shared_ptr<vector<Elem>> returnResults;
  //! For BOOLEAN
  explicit SelectClause(ReturnType returnType);
  //! For Tuple
  SelectClause(ReturnType returnType, shared_ptr<vector<Elem>> returnResults);

  bool isBoolean() const;

  static const int ELEM_SYN_IDX = 0;
  static const int ELEM_ATTR_REF_IDX = 1;

  bool operator==(const SelectClause& clause) const;
  shared_ptr<Table> accept(shared_ptr<IVisitor> visitor) override;
};
}  // namespace QB

#endif  // SPA_SELECTCLAUSE_H
