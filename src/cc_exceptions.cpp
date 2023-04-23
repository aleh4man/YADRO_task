#include <exception>
#include <string>
#include "cc_exceptions.h"

CCExceptionIncorrectInput::CCExceptionIncorrectInput(const std::string& str){incorrect_string = str;}

const char* CCExceptionIncorrectInput::what() const noexcept{
    return incorrect_string.c_str();
}
