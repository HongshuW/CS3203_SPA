//
// Created by hongshu wang on 19/9/22.
//

#ifndef SPA_AFFECTSTABLE_H
#define SPA_AFFECTSTABLE_H

#include "Cachable.h"
#include "RelationTable.h"

class AffectsTable : public Cachable {
public:
    explicit AffectsTable();
    void clearCache() override;
};


#endif //SPA_AFFECTSTABLE_H
