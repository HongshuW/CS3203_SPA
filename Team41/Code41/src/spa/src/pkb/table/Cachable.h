//
// Created by hongshu wang on 7/10/22.
//

#ifndef SPA_CACHABLE_H
#define SPA_CACHABLE_H

class Cachable {
public:
    bool queried = false;

    bool isQueried();
    void setQueried(bool status);
    virtual void clearCache();
};


#endif //SPA_CACHABLE_H
