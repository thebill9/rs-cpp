#include "File.h"

size_t File::getSize() const
{
	return size;
}

void File::setSize(const size_t &value)
{
	size = value;
}

string File::getFileName() const
{
	return fileName;
}

void File::setFileName(const string &value)
{
	fileName = value;
}

char *File::getData() const
{
	return data;
}

void File::setData(char *value)
{
	data = value;
}

File::File()
{

}

File::File(char *data, string fileName, size_t size)
{
	this->fileName = fileName;
	this->data = data;
	this->size = size;
}
