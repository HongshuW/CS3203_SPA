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
    static Table * getFollowsStar();
    static Table * getParent();
    static Table * getParentStar();
    static Table * getUses();
    static Table * getModifies();

private:
    static Table followsTable;
    static Table followsStarTable;
    static Table parentTable;
    static Table parentStarTable;
    static Table usesTable;
    static Table modifiesTable;

    static vector<string> followsTableHeader;
    static vector<string> followsStarTableHeader;
    static vector<string> parentTableHeader;
    static vector<string> parentStarTableHeader;
    static vector<string> usesTableHeader;
    static vector<string> modifiesTableHeader;
};

#endif //SPA_RELATIONSHIPMANAGER_H
