#pragma once
#include <exception>

using namespace std;
class NullPointerException : public exception
{
public:
	NullPointerException();
	virtual const char* what() const throw();
};
