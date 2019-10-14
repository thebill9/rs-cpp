#include "Port.h"
#include <iostream>
#include <vector>
string Port::getPortName() const
{
	return portName;
}

void Port::setPortName(const string &value)
{
	portName = value;
}

DWORD Port::getErrors() const
{
	return errors;
}

DWORD Port::getCbInQue()
{
	return comstat.cbInQue;
}

void Port::clearErrors()
{
	ClearCommError(portId, &errors, &comstat);
}

void Port::setRTS()
{
	EscapeCommFunction(portId, SETRTS);
}

void Port::clearRTS()
{
	EscapeCommFunction(portId, CLRRTS);
}

bool Port::isOpen()
{
	return this->opened;
}

bool Port::canRead()
{
	return !this->waiting2Write && !this->isReading;
}

Port::Port()
{
	this->isReading = false;
	this->waiting2Write = false;
}

bool Port::open()
{
	COMMPROP comprop;
	comprop.wPacketLength = sizeof(COMMPROP);
	portId = CreateFile((LPCSTR)portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (portId != INVALID_HANDLE_VALUE)
	{
		GetCommState(portId, &dcb);
		dcb.BaudRate = CBR_128000;
		dcb.DCBlength = sizeof(dcb);
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fParity = TRUE;
		dcb.Parity = ODDPARITY;
		dcb.StopBits = TWOSTOPBITS;
		dcb.fAbortOnError = true;
		dcb.ByteSize = 7;
		dcb.fDtrControl = 1;
		SetCommState(portId, &dcb);

		DWORD eventMask;
		GetCommMask(portId, &eventMask);
		SetCommMask(portId, EV_BREAK | EV_CTS | EV_RLSD | EV_TXEMPTY);

		GetCommProperties(portId, &comprop);
		COMMTIMEOUTS commTimeouts;
		GetCommTimeouts(portId, &commTimeouts);
		commTimeouts.ReadTotalTimeoutConstant = 1;
		commTimeouts.ReadIntervalTimeout = 1;
		commTimeouts.ReadTotalTimeoutMultiplier = 200;
		SetCommTimeouts(portId, &commTimeouts);

		this->opened = true;
		return true;
	}
	else
		this->opened = false;
	return false;

}

bool Port::open(string portName)
{
	this->setPortName(portName);
	return this->open();

}

bool Port::close()
{
	if (this->portId == INVALID_HANDLE_VALUE)
		return false;
	return CloseHandle(portId) != 0;
}

void Port::read(char *&data, size_t &bytesToRead, chrono::time_point<chrono::system_clock> *&start, chrono::time_point<chrono::system_clock> *&end)
{
	if (portId == INVALID_HANDLE_VALUE) {
		throw new PortNotOpenException();
	}
	this->clearErrors();

	//cout << "wait 4 comm\n";
	//WaitCommEvent(portId, &eventMask, nullptr);
	char buff = '\0';
	DWORD bytesRead;

	// get header
	string header = "";
	start = new chrono::time_point<chrono::system_clock>();
	end = new chrono::time_point<chrono::system_clock>();
	while (true) {
		ReadFile(portId, &buff, 1, &bytesRead, NULL);
		if (buff) {
			if (!this->isReading) {
			}
			this->isReading = true;
			header += buff;
		}
		if (buff == ';') {
			break;
		}
		if (!this->isReading && this->waiting2Write) {
			return;
		}
	}

	size_t beginH = header.find(":");
	size_t endH = header.find(";");
	size_t dataSize = (size_t)stoi(header.substr(beginH + 1, endH - beginH));

	size_t totalRead = 0;
	data = new char[dataSize];
	bytesToRead = dataSize;
	*start = chrono::system_clock::now();
	while (dataSize > 0) {
		char *part = &data[totalRead];
		ReadFile(portId, part, dataSize, &bytesRead, 0);
		totalRead += (size_t)bytesRead;
		dataSize -= (size_t)bytesRead;
	}
	*end = chrono::system_clock::now();
	this->isReading = false;
	
}

size_t Port::write(char *data, size_t bytesToWrite, chrono::time_point<chrono::system_clock> *&start, chrono::time_point<chrono::system_clock> *&end)
{
	if (portId == INVALID_HANDLE_VALUE) {
		throw new PortNotOpenException;
	}

	start = new chrono::time_point<chrono::system_clock>();
	end = new chrono::time_point<chrono::system_clock>();

	this->waiting2Write = true;
	size_t totalWritten = 0;
	DWORD bytesWritten;
	// sending header
	string header = "read:" + to_string(bytesToWrite) + ";";
	char *cheader = new char[header.size()];
	strcpy(cheader, header.c_str());
	size_t writtenHeader = 0;
	DWORD headerToWrite = header.size();
	while (headerToWrite > 0) {
		DWORD headerWritten;
		WriteFile(portId, &cheader[writtenHeader], headerToWrite, &headerWritten, NULL);
		headerToWrite -= headerWritten;
	}

	*start = chrono::system_clock::now();
	while (totalWritten != bytesToWrite) {
		DWORD toWrite = bytesToWrite - totalWritten;
		DWORD written;
		WriteFile(portId, &data[totalWritten], toWrite, &written, NULL);
		totalWritten += (size_t)written;
	}

	WriteFile(portId, &endOfMessage, 1, &bytesWritten, NULL);
	WriteFile(portId, &endOfMessage, 1, &bytesWritten, NULL);

	*end = chrono::system_clock::now();
	this->waiting2Write = false;
	return totalWritten;
}

Port::~Port()
{
	close();
}
