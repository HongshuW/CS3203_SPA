//
// Created by Xingchen Lin on 15/9/22.
//

#ifndef SPA_SPAMANAGER_H
#define SPA_SPAMANAGER_H

#include "pkb/DataModifier.h"
#include "pkb/DataRetriever.h"
#include <memory>


class SPAManager {
    //! Entry point of the program, each program should have one SPAManager
private:
    shared_ptr<DataModifier> dataModifier;
    shared_ptr<DataRetriever> dataRetriever;
public:
    //! To parse source program
    void parse(string& filename);
    //! To evaluate query
    vector<string> evaluate(string& query);
};


#endif //SPA_SPAMANAGER_H
