#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#include<string>

class CustomException : public std::exception {
private:
    std::string message;

public:
    CustomException(const std::string& _message);
    const std::string getMessage() const;
};

#endif