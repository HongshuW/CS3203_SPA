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
    static Table followsTable;
    static Table followsStarTable;
    static Table parentTable;
    static Table parentStarTable;
    static Table UsesTable;
    static Table ModifiesTable;
};

#endif //SPA_RELATIONSHIPMANAGER_H
