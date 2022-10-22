//
// Created by hongshu wang on 19/9/22.
//

#ifndef SPA_NEXTTABLE_H
#define SPA_NEXTTABLE_H

#include "Cachable.h"

class NextTable : public Cachable {
 public:
  explicit NextTable();
  void clearCache() override;
};

#endif  // SPA_NEXTTABLE_H
