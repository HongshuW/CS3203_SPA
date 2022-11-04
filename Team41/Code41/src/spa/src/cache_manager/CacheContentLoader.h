//
// Created by hongshu wang on 4/11/22.
//

#ifndef SPA_CACHECONTENTLOADER_H
#define SPA_CACHECONTENTLOADER_H

#include "design_extractor/DesignExtractorRetriever.h"

using namespace DE;

class CacheContentLoader {
public:
    typedef shared_ptr<vector<string>> (CacheContentLoader::*partialGetter)(int);
    typedef shared_ptr<list<vector<string>>> (CacheContentLoader::*fullGetter)();
    typedef bool (CacheContentLoader::*exactGetter)(int, int);

    static const int WILDCARD_STMTNO;
    shared_ptr<DesignExtractorRetriever> designExtractorRetriever;

    explicit CacheContentLoader(
            shared_ptr<DesignExtractorRetriever> designExtractorRetriever);

    shared_ptr<list<vector<string>>> getNextTRelations();
    shared_ptr<vector<string>> getNextTStatements(int stmtNo);
    shared_ptr<vector<string>> getPreviousTStatements(int stmtNo);
    bool getNextTResult(int precedingStatement, int ensuingStatement);
    shared_ptr<list<vector<string>>> getAffectsRelations();
    shared_ptr<vector<string>> getAffectedStatements(int stmtNo);
    shared_ptr<vector<string>> getAffectingStatements(int stmtNo);
    bool getAffectsResult(int affectingStatement, int affectedStatement);
    shared_ptr<list<vector<string>>> getAffectsTRelations();
    shared_ptr<vector<string>> getAffectedTStatements(int stmtNo);
    shared_ptr<vector<string>> getAffectingTStatements(int stmtNo);
    bool getAffectsTResult(int affectingStatement, int affectedStatement);
};


#endif //SPA_CACHECONTENTLOADER_H
