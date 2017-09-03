#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException()
{

}

const char *FileNotFoundException::what() const throw()
{
	return "File not found";
}
