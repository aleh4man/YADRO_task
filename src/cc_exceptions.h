#ifndef _CC_EXCEPTIONS_
#define _CC_EXCEPTIONS_

#include <exception>
#include <string>

class CCExceptionIncorrectInput : std::exception{
    private:
        std::string incorrect_string;
    public:
    CCExceptionIncorrectInput(const std::string& str){
        incorrect_string = str;
    }

    const char* what() const noexcept{
        return incorrect_string.c_str();
    }
};

#endif