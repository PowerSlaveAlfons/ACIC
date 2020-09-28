#include "PSAString.h"
#include <iostream>


int main()
{
    PSAString* str = new PSAString("Hello");
    PSAString* str3 = new PSAString("World");
    PSAString* str2 = str->Concetonate(str3);

    std::cout << "str: " << str->c_str() << std::endl;
    std::cout << "str3: " << str3->c_str() << std::endl;
    std::cout << "str2: " << str2->c_str() << std::endl;

    std::cout << "Length of str2: " << str2->GetLength() << std::endl;

    //str2 = str2->Concetonate(str3);

    delete str;
    delete str2;
    delete str3;
    return 0;
}