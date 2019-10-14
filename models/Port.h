#pragma once
#include <windows.h>
#include "../lib/FileNotFoundException.h"
#include "../lib/PortNotOpenException.h"
#include <string>
#include <chrono>

using namespace std;

class Port
{
	HANDLE portId;
	DCB dcb;
	string portName;
	DWORD errors;
	COMSTAT comstat;
	DWORD eventMask;
	bool opened;
	bool isReading;
	bool waiting2Write;
	static const char endOfMessage = '\n';
public:
	Port();
	bool open();
	bool open(string portName);
	bool close();
	void read(char *& data, size_t & bytesToRead, chrono::time_point<chrono::system_clock>*& start, chrono::time_point<chrono::system_clock>*& end);
	size_t write(char * data, size_t bytesToWrite, chrono::time_point<chrono::system_clock>*& start, chrono::time_point<chrono::system_clock>*& end);
	~Port();
	string getPortName() const;
	void setPortName(const string &value);
	DWORD getErrors() const;
	DWORD getCbInQue();
	void clearErrors();
	void setRTS();
	void clearRTS();
	bool isOpen();
	bool canRead();
};