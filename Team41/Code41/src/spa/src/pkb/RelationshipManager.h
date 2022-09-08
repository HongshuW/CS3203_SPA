//
// Created by hongshu wang on 5/9/22.
//

#ifndef SPA_RELATIONSHIPMANAGER_H
#define SPA_RELATIONSHIPMANAGER_H

#include <stdio.h>
#include "pkb/table/FollowsTable.h"
#include "pkb/table/ModifiesTable.h"
#include "pkb/table/ParentTable.h"
#include "pkb/table/Table.h"
#include "pkb/table/UsesTable.h"

using namespace std;

class RelationshipManager {
public:
    static FollowsTable * getFollows();
    static FollowsTable * getFollowsT();
    static ParentTable * getParent();
    static ParentTable * getParentT();
    static UsesTable * getUsesS();
    static UsesTable * getUsesP();
    static ModifiesTable * getModifiesS();
    static ModifiesTable * getModifiesP();

private:
    static FollowsTable followsTable;
    static FollowsTable followsTTable;
    static ParentTable parentTable;
    static ParentTable parentTTable;
    static UsesTable usesSTable;
    static UsesTable usesPTable;
    static ModifiesTable modifiesSTable;
    static ModifiesTable modifiesPTable;
};

#endif //SPA_RELATIONSHIPMANAGER_H
