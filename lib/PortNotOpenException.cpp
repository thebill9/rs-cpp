#include "PortNotOpenException.h"

PortNotOpenException::PortNotOpenException()
{

}

const char *PortNotOpenException::what() const throw()
{
	return "Port not open";
}
