#include "custom_exceptions.h"

CustomException::CustomException(const std::string& _message) : message(_message) {}

const std::string CustomException::getMessage() const {
    return message;
}
