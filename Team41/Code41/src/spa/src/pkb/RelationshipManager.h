//
// Created by hongshu wang on 5/9/22.
//

#ifndef SPA_RELATIONSHIPMANAGER_H
#define SPA_RELATIONSHIPMANAGER_H

#include <stdio.h>
#include "Table.h"

using namespace std;

class RelationshipManager {
public:
    static Table * getFollows();
    static Table * getFollowsT();
    static Table * getParent();
    static Table * getParentT();
    static Table * getUsesS();
    static Table * getUsesP();
    static Table * getModifiesS();
    static Table * getModifiesP();

private:
    static Table followsTable;
    static Table followsTTable;
    static Table parentTable;
    static Table parentTTable;
    static Table usesSTable;
    static Table usesPTable;
    static Table modifiesSTable;
    static Table modifiesPTable;

    static vector<string> followsTableHeader;
    static vector<string> followsTTableHeader;
    static vector<string> parentTableHeader;
    static vector<string> parentTTableHeader;
    static vector<string> usesSTableHeader;
    static vector<string> usesPTableHeader;
    static vector<string> modifiesSTableHeader;
    static vector<string> modifiesPTableHeader;
};

#endif //SPA_RELATIONSHIPMANAGER_H
