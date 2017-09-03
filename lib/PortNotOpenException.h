#pragma once
#include <exception>

using namespace std;

class PortNotOpenException : public exception
{
public:
	PortNotOpenException();
	virtual const char* what() const throw();
};

