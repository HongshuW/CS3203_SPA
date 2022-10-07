//
// Created by hongshu wang on 7/10/22.
//

#include "CacheManager.h"

CacheManager::CacheManager(shared_ptr<DesignExtractor> designExtractor) {
    this->designExtractor = designExtractor;
}

list<vector<string>> CacheManager::getNextTRelations() {
    return designExtractor->getAllNextStarRelations();
}

list<vector<string>> CacheManager::getAffectsRelations() {
    return designExtractor->getAllAffectsRelations();
}

list<vector<string>> CacheManager::getAffectsTRelations() {
    return designExtractor->getAllAffectsStarRelations();
}
