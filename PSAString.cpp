#include "PSAString.h"

/*
* This helper method returns the length of a given C-Style String.
* Since C-Style Strings are null terminated, it loops through the string until the null terminator has been found.
*/

uint32_t PSAString::strLen(const char *chararr)
{
    uint32_t len = 0;
    while (chararr[len] != '\0')
        len++;
    return len;
}

/*
* The constructor uses an initializer list, so the length can already be set before the rest of it is executed.
*/

PSAString::PSAString(const char *data) : length(strLen(data))
{

    /*
    * The internal char array lives on the heap in order for it to be accessible by outsiders (via the c_str() method).
    * The length is offset by +1 due to the null terminator.
    */
    this->internal_str = std::shared_ptr<char[]>(new char[length + 1]);

    /*
    * In order for the function argument to be const, we need to actually copy the values into the new internal char array,
    * otherwise the internal data might be changed which would lead to unintended side effects.
    */

    for (int i = 0; i < length; i++)
    {
        this->internal_str.get()[i] = data[i];
    }
    this->internal_str.get()[length] = '\0';

}

/*
* Returns a new String that resembles the string it operates on (this) with the argument appended to the end of it.
*/
PSAString *PSAString::Concetonate(PSAString *other)
{
    /*
    * There is no need to put a temporary array that's only being used by this very method onto the heap.
    */

    char temp[this->GetLength() + other->GetLength() + 1];

    char *oldArr = this->internal_str.get();
    char *otherArr = other->internal_str.get();

    auto oldPtr = std::make_shared<char*>(this->internal_str.get());

    uint32_t oldLength = this->GetLength();
    uint32_t newLength = oldLength + other->GetLength();

    /*
    * Using two loops (one after another, so still O(n)), the values of both arrays are copied into the new array.
    */
    for (int i = 0; i < oldLength; i++)
        temp[i] = oldArr[i];
    for (int i = oldLength; i < newLength; i++)
        temp[i] = otherArr[i - oldLength];

    temp[newLength] = '\0';

    /*
    * Returns a new string constructed with the temporary array that's just been built.
    */
    PSAString* returned = new PSAString(temp);
    oldPtr.reset();
    return returned;


}

const uint32_t PSAString::GetLength()
{
    return this->length;
}

const char *PSAString::c_str()
{
    return this->internal_str.get();
}