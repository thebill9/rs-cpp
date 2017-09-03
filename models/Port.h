#pragma once
#include <windows.h>
#include "../lib/FileNotFoundException.h"
#include "../lib/PortNotOpenException.h"
#include <string>

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
public:
	Port();
	bool open();
	bool open(string portName);
	bool close();
	void read(char *data, size_t &bytesToRead);
	void rawRead(char *data, size_t &bytesToRead);
	size_t write(char *data, size_t bytesToWrite);
	~Port();
	string getPortName() const;
	void setPortName(const string &value);
	DWORD getErrors() const;
	DWORD getCbInQue();
	void clearErrors();
	void setRTS();
	void clearRTS();
	bool isOpen();
};