#include "Operation.h"

Operation::Operation(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end, string name)
{
	this->start = start;
	this->end = end;
	this->name = name;
}
