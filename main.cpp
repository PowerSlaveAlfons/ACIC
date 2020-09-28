#include "PSAString.h"
#include <iostream>


int main()
{
    char World[6] = "World";
    

    /*
    * Using smart pointers here in order to not have concetonate leak if I assign the 
    * returned result to the argument
    */
    auto str = std::shared_ptr<PSAString>(new PSAString("Hello"));
    auto str3 = std::shared_ptr<PSAString>(new PSAString(World));
    auto str2 = std::shared_ptr<PSAString>(str.get()->Concetonate(str3.get()));

    std::cout << "str: " << str.get()->c_str() << std::endl;
    std::cout << "str3: " << str3.get()->c_str() << std::endl;
    std::cout << "str2: " << str2.get()->c_str() << std::endl;

    std::cout << "Length of str2: " << str2.get()->GetLength() << std::endl;

    str2 = std::shared_ptr<PSAString>(str2.get()->Concetonate(str3.get()));

    /*
    * -fsanitize reports absolutely nothing, valgrind only reports 324 bytes as still reachable;
    * According to my research this is memory that is immediately given back to the OS anyway on anything valgrind runs, so it 
    * should not be an issue.
    */

    return 0;
}