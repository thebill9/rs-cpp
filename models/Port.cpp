#include "Port.h"

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

Port::Port()
{

}

bool Port::open()
{
	COMMPROP comprop;
	comprop.wPacketLength = sizeof(COMMPROP);
	portId = CreateFile((LPCSTR)portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (portId != INVALID_HANDLE_VALUE)
	{
		GetCommState(portId, &dcb);
		dcb.BaudRate = CBR_1200;
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
		commTimeouts.ReadTotalTimeoutMultiplier = 2000;
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

void Port::read(char *data, size_t &bytesToRead)
{
	if (portId == INVALID_HANDLE_VALUE) {
		throw new PortNotOpenException();
	}
	this->clearErrors();

	size_t totalRead = 0;
	data = new char[comstat.cbInQue];
	while (comstat.cbInQue > 0) {
		char *part = &data[totalRead];
		DWORD bytesRead;
		ReadFile(portId, part, comstat.cbInQue, &bytesRead, 0);
		totalRead += (size_t)bytesRead;
	}
	bytesToRead = totalRead;
}

void Port::rawRead(char *data, size_t &bytesToRead)
{
	if (portId == INVALID_HANDLE_VALUE) {
		throw new PortNotOpenException;
	}
	this->clearErrors();
	DWORD bytesRead = 0;
	ReadFile(portId, data, bytesToRead, &bytesRead, 0);
	bytesToRead = (size_t)bytesRead;
}

size_t Port::write(char *data, size_t bytesToWrite)
{
	if (portId == INVALID_HANDLE_VALUE) {
		throw new PortNotOpenException;
	}

	size_t totalWritten = 0;
	DWORD bytesWritten;
	while (totalWritten != bytesToWrite) {
		char *p = &data[totalWritten];
		size_t toWrite = bytesToWrite - totalWritten;
		WriteFile(portId, p, toWrite, &bytesWritten, 0);
		totalWritten += (size_t)bytesWritten;
		WaitCommEvent(portId, &eventMask, nullptr);
	}
	return totalWritten;
}

Port::~Port()
{
	close();
}
