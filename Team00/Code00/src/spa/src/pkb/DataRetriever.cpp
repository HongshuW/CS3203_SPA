//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"
#include "EntityManager.h"

Table DataRetriever::getVariables() {
    return *EntityManager::getVariables();
}
