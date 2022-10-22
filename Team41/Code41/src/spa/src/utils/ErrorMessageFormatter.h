//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_ERRORMESSAGEFORMATTER_H
#define SPA_ERRORMESSAGEFORMATTER_H
#include <string>

using namespace std;
class ErrorMessageFormatter {
  static const string EXCEPTION_ERROR_MESSAGE_EXPECT;
  static const string EXCEPTION_ERROR_MESSAGE_GOT;

 public:
  static string formatErrorMessage(const string& expect, const string& actual);
};

#endif  // SPA_ERRORMESSAGEFORMATTER_H
