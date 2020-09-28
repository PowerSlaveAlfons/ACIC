#pragma once
#include <stdint.h>
#include <iostream>
#include <memory>
#include <utility>

class PSAString
{
private:
    std::shared_ptr<char[]> internal_str; // internally, PSAStrings use the C method of storing chars in arrays with a null terminator
    uint32_t length; // size is not specified exactly, fixed 32bit width integers are used for consistency across systems and compilers; negative size is illogical, therefore unsigned
    static uint32_t strLen(const char*); // this does not operate on a PSAString, and therefore has no reason not to be static, it's more of a helper method that fits into the class
public:
    PSAString(const char*);

    PSAString* Concetonate(PSAString*);
    const char* c_str();
    const uint32_t GetLength();
};