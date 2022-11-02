//
// Created by Nafour on 30/9/22.
//

#ifndef SPA_IVISITORHOST_H
#define SPA_IVISITORHOST_H

#include <variant>

#include "pkb/table/Table.h"

class IVisitor;
class IVisitorHost {
 public:
  virtual ~IVisitorHost() = default;
  virtual shared_ptr<Table> accept(shared_ptr<IVisitor> visitor) = 0;
};

#endif  // SPA_IVISITORHOST_H
