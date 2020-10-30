#include <iostream>
#include <memory>

class PSAString
{
private:
    std::shared_ptr<char[]> internal_str;       // internally, PSAStrings use the C method of storing chars in arrays with a null terminator
    uint32_t length;                            // size is not specified exactly, fixed 32bit width integers are used for consistency across systems and compilers; negative size is illogical, therefore unsigned
    static uint32_t strLen(const char *chararr) // this does not operate on a PSAString, and therefore has no reason not to be static, it's more of a helper method that fits into the class
    {
        uint32_t len = 0;
        while (chararr[len] != '\0')
            len++;
        return len;
    }

public:
    PSAString() : length(0)
    {
        this->internal_str = std::shared_ptr<char[]>(new char[1]);
        this->internal_str.get()[0] = '\0';
    }
    
    PSAString(const char *data) : length(strLen(data))
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

        for (uint32_t i = 0; i < length; i++)
        {
            this->internal_str.get()[i] = data[i];
        }
        this->internal_str.get()[length] = '\0';
    }

    /* Default constructor that just takes a length */

    PSAString(uint32_t length) : length(length - 1)
    {
        this->internal_str = std::shared_ptr<char[]>(new char[length + 1]);
    }

    /* Copy constructor that deepcopies the chars from other into a new Object */

    PSAString(const PSAString &other) : length(other.GetLength())
    {
        this->internal_str = std::shared_ptr<char[]>(new char[length + 1]);

        const char *data = other.c_str();
        for (uint32_t i = 0; i < length; i++)
        {
            this->internal_str.get()[i] = data[i];
        }
        this->internal_str.get()[length] = '\0';
    }

    PSAString &operator=(const PSAString &other)
    {

        /*  If I'm copying the same object ... well, don't copy it, waste of time */
        if (this != &other)
        {
            /* Assign the length of the internal array */
            if (this->length != other.GetLength())
                this->length = other.GetLength();

            this->internal_str = std::shared_ptr<char[]>(new char[this->length + 1]);

            /* Deepcopy the characters into the new object */
            for (uint32_t i = 0; i < length; i++)
            {
                this->internal_str.get()[i] = other.internal_str.get()[i];
            }
            this->internal_str.get()[length] = '\0';
        }

        return *this;
    }

    PSAString *Concatenate(const PSAString *other)
    {
        /* Fixed the unnecessary array */

        PSAString *Returned = new PSAString(this->GetLength() + other->GetLength() + 1);

        char *oldArr = this->internal_str.get();
        char *otherArr = other->internal_str.get();

        uint32_t oldLength = this->GetLength();
        uint32_t newLength = oldLength + other->GetLength();

        /*
		* Using two loops (one after another, so still O(n)), the values of both arrays are copied into the new array.
		*/

        for (uint32_t i = 0; i < oldLength; i++)
            Returned->internal_str.get()[i] = oldArr[i];
        for (uint32_t i = oldLength; i < newLength; i++)
            Returned->internal_str.get()[i] = otherArr[i - oldLength];

        Returned->internal_str.get()[newLength] = '\0';
        return Returned;
    }

    /* This should fix the issue of creating a shared_ptr from a raw pointer - Overloading two methods that only differ by return type is sadly not possible */

    std::shared_ptr<PSAString> ConcetaneteShared(const PSAString *other)
    {
        /*
		* There is no need to put a temporary array that's only being used by this very method onto the heap.
		*/

        auto Returned = std::make_shared<PSAString>(this->GetLength() + other->GetLength() + 1);

        char *oldArr = this->internal_str.get();
        char *otherArr = other->internal_str.get();

        uint32_t oldLength = this->GetLength();
        uint32_t newLength = oldLength + other->GetLength();

        /*
		* Using two loops (one after another, so still O(n)), the values of both arrays are copied into the new array.
		*/

        for (uint32_t i = 0; i < oldLength; i++)
            Returned->internal_str.get()[i] = oldArr[i];
        for (uint32_t i = oldLength; i < newLength; i++)
            Returned->internal_str.get()[i] = otherArr[i - oldLength];

        Returned->internal_str.get()[newLength] = '\0';
        return Returned;
    }

    uint32_t GetLength() const
    {
        return this->length;
    }

    const char *c_str() const
    {
        return this->internal_str.get();
    }
};

int main()
{
    char World[6] = "World";

    /*
	* Using smart pointers here in order to not have concetonate leak if I assign the
	* returned result to the argument
	*/

    auto str = std::make_shared<PSAString>("Hello");
    auto str3 = std::make_shared<PSAString>(World);
    auto str2 = std::shared_ptr<PSAString>(str.get()->ConcetaneteShared(str3.get()));

    std::cout << "str: " << str.get()->c_str() << std::endl;
    std::cout << "str3: " << str3.get()->c_str() << std::endl;
    std::cout << "str2: " << str2.get()->c_str() << std::endl;

    std::cout << "Length of str2: " << str2.get()->GetLength() << std::endl;

    /*
	* -fsanitize reports absolutely nothing, valgrind only reports 324 bytes as still reachable;
	* According to my research this is memory that is immediately given back to the OS anyway on anything valgrind runs, so it
	* should not be an issue. - This seems to be gone now.
    * Should actually work in clang and MSVC now.
	*/

    /* UE2 kinda starts here */
    std::cout << "&&&___UE2___&&&" << std::endl;
    auto UE2_1 = std::make_shared<PSAString>("Hello");
    auto UE2_2 = std::shared_ptr<PSAString>(UE2_1.get()->ConcetaneteShared(str3.get()));
    auto UE2_Original = UE2_2;
    auto UE2_Copy = UE2_Original;
    std::cout << UE2_Copy.get()->c_str() << std::endl;

    /* 
     * ==4929== All heap blocks were freed -- no leaks are possible
     * Greetings from valgrind
     * 
     * Developed in g++ 7.5.0
     * Tested in clang 6.0.0 and whatever MSVC version VS Community 2019 uses right now 
     */

    return 0;
}