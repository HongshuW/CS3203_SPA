//
// Created by Aaron on 11/10/22.
//

#ifndef SPA_SAVEEXTRACTEDRELATION_H
#define SPA_SAVEEXTRACTEDRELATION_H

#include "SaveExtractedRelationVisitor.h"

class SaveExtractedRelation {
public:
    SaveExtractedRelation();
    virtual ~SaveExtractedRelation() = default;
    virtual void accept(SaveExtractedRelationVisitor visitor) = 0;
};


#endif //SPA_SAVEEXTRACTEDRELATION_H
