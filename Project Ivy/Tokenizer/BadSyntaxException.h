#pragma once
#include <string>
#include <exception>
class BadSyntaxException: public std::exception
{
public:
	BadSyntaxException(int lineNumber, int linePosition);
	~BadSyntaxException();
	virtual const char* what() const throw(){
		return exceptionMessage.c_str();
	}
private:
	std::string exceptionMessage;
};

