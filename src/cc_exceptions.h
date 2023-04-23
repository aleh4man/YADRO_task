#ifndef _CC_EXCEPTIONS_
#define _CC_EXCEPTIONS_

#include <exception>
#include <string>

class CCExceptionIncorrectInput : std::exception{
    private:
        std::string incorrect_string;
    public:
    CCExceptionIncorrectInput(const std::string&);
    const char* what() const noexcept;
};

#endif