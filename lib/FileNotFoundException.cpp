#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException()
{

}

const char *FileNotFoundException::what() const throw()
{
	return "Plik nie istnieje";
}
