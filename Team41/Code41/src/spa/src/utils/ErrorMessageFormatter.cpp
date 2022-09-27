//
// Created by Nafour on 27/9/22.
//

#include "ErrorMessageFormatter.h"

const string ErrorMessageFormatter:: EXCEPTION_ERROR_MESSAGE_EXPECT = "Expect: ";
const string ErrorMessageFormatter:: EXCEPTION_ERROR_MESSAGE_GOT = ", got: ";

string ErrorMessageFormatter::formatErrorMessage(const string &expect, const string &actual) {
    return EXCEPTION_ERROR_MESSAGE_EXPECT
           + expect
           + EXCEPTION_ERROR_MESSAGE_GOT
           + actual;
}
