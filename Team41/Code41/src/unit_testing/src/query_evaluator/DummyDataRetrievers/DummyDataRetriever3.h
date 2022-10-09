//
// Created by Nafour on 9/10/22.
//

#ifndef SPA_DUMMYDATARETRIEVER3_H
#define SPA_DUMMYDATARETRIEVER3_H

#include "pkb/DataRetriever.h"

/*
 * code 6 from wiki
 *
 *  procedure First {
      read x;
      read z;
      call Second; }

      procedure Second {
01        x = 0;
02        i = 5;
03        while (i!=0) {
04            x = x + 2*y;
05            call Third;
06            i = i - 1; }
07        if (x==1) then {
08            x = x+1; }
          else {
09            z = 1; }
10        z = z + x + i;
11        y = z + 2;
12        x = x * y + z; }

      procedure Third {
          z = 5;
          v = z;
          print v; }
 */

class DummyDataRetriever3  : public DataRetriever {

};


#endif //SPA_DUMMYDATARETRIEVER3_H
