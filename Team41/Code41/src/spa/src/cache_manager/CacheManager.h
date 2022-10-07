//
// Created by hongshu wang on 7/10/22.
//

#ifndef SPA_CACHEMANAGER_H
#define SPA_CACHEMANAGER_H

#include "./design_extractor/DesignExtractor.h"

using namespace DE;

class CacheManager {
public:
    shared_ptr<DesignExtractor> designExtractor;

    explicit CacheManager(shared_ptr<DesignExtractor> designExtractor);

    list<vector<string>> getNextTRelations();
    list<vector<string>> getAffectsRelations();
    list<vector<string>> getAffectsTRelations();
};


#endif //SPA_CACHEMANAGER_H
