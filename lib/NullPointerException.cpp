#include "NullPointerException.h"

NullPointerException::NullPointerException()
{

}

const char *NullPointerException::what() const throw()
{
	return "Null pointer exception";
}
