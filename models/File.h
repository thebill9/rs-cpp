#pragma once
#include <string>
#include <memory>
#include <vector>

using namespace std;
class File
{
	char *data;
	string fileName;
	size_t size;
public:
	File();
	File(char *data, string fileName, size_t size);
	size_t getSize() const;
	void setSize(const size_t &value);
	string getFileName() const;
	void setFileName(const string &value);
	char *getData() const;
	void setData(char *value);
	~File() {
		delete[] data;
	}
};
