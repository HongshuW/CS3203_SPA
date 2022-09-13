//
// Created by Nafour on 13/9/22.
//

#ifndef SPA_QETESTUTILS_H
#define SPA_QETESTUTILS_H

#include <vector>
#include <string>

using namespace std;

namespace QETest {

    class QETestUtils {
    public:
        static bool containsSameElement(vector<string> vec1, vector<string> vec2);
    };

} // QETest

#endif //SPA_QETESTUTILS_H
