#pragma once
#include <exception>

using namespace std;
class FileNotFoundException : public exception
{
public:
	FileNotFoundException();
	virtual const char* what() const throw();
};