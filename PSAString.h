#pragma once
#include <stdint.h>
#include <iostream>

class PSAString
{
private:
    char* internal_str;
    uint32_t length; // size is not specified exactly
    static uint32_t strLen(const char*);

public:
    PSAString(const char*);

    PSAString* Concetonate(const PSAString*);
    const char* c_str();
    const uint32_t GetLength();
};