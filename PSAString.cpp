#include "PSAString.h"

uint32_t PSAString::strLen(const char* chararr)
{
    uint32_t len = 0;
    while (chararr[len] != '\0')
        len++;
    return len;
}

PSAString::PSAString(const char* data) : length(strLen(data))
{
    this->internal_str = new char[length];

    for (int i = 0; i < length; i++)
    {
        this->internal_str[i] = data[i];
    }
}

PSAString* PSAString::Concetonate(const PSAString* other)
{
    char* temp = new char[this->GetLength() + other->GetLength()];
    char* oldArr = this->internal_str;
    char* otherArr = other->internal_str;


    uint32_t oldLength = this->GetLength();
    uint32_t newLength = oldLength + other->GetLength();
    std::cout << "OldL " << oldLength << std::endl;
    std::cout << "NewL. " << newLength << std::endl;


    for (int i = 0; i < oldLength; i++)
        temp[i] = oldArr[i];
    for (int i = oldLength; i < newLength; i++)
        temp[i] = otherArr[i-oldLength];

    PSAString* Returned = new PSAString(temp);
    return Returned;
}

const uint32_t PSAString::GetLength()
{
    return strLen(this->internal_str);
}

const char* PSAString::c_str()
{
    return this->internal_str;
}