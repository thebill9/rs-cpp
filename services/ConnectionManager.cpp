#include "ConnectionManager.h"


using namespace std;


size_t ConnectionManager::sendData(char *data, size_t size, chrono::time_point<chrono::system_clock> *&start, chrono::time_point<chrono::system_clock> *&end)
{
	if (!data) {
		throw new NullPointerException;
	}
	size_t bytesWritten = this->port->write(data, size, start, end);
	port->clearRTS();
	return bytesWritten;
}

void ConnectionManager::parse2send(File *file, char *&output, size_t & size)
{
	if (file == nullptr || file->getData() == nullptr) {
		throw new NullPointerException;
	}
	string message = "file:" + file->getFileName();
	message += "\tdata:";
	size = message.size() + file->getSize();
	output = new char[size];
	strcpy(output, message.c_str());
	strcat(output, file->getData());
}

File *ConnectionManager::parseData(char *data, size_t size)
{
	char namePrefix[] = "file:";
	char dataPrefix[] = "\tdata:";
	char *namePointer;
	char *dataPointer;
	char *endNamePointer;
	namePointer = strstr(data, namePrefix) + sizeof(namePrefix) - 1;
	endNamePointer = strstr(data, dataPrefix);
	dataPointer = strstr(data, dataPrefix) -1;
	string fileName = "";
	if (namePointer && endNamePointer) {
		while (namePointer != endNamePointer) {
			fileName += namePointer[0];
			++namePointer;
		}
	}
	size_t sizeOfData = size + 2 - sizeof(namePrefix) - sizeof(dataPrefix) - fileName.size();
	char *fileData = new char[sizeOfData];
	if (dataPointer) {
		memcpy(fileData, dataPointer + sizeof(dataPrefix), sizeOfData);
	}
	 File *file(new File(fileData, fileName, sizeOfData));
	 return file;
}

ConnectionManager::ConnectionManager()
{
	this->port = nullptr;
}

bool ConnectionManager::isPortOpen()
{
	return this->port->isOpen();
}

bool ConnectionManager::init(string portName)
{
	if (this->port == nullptr) {
		this->port = new Port();
	}

	return port->open(portName);
}

size_t ConnectionManager::sendFile(File *file, chrono::time_point<chrono::system_clock> *&start, chrono::time_point<chrono::system_clock> *&end)
{
	if (!file) {
		throw new NullPointerException();
	}

	char *data = nullptr;
	size_t size = 0;
	parse2send(file, data, size);
	size_t sentData = this->sendData(data, size, start, end);
	delete file;
	return sentData;
}

File *ConnectionManager::receiveFile(chrono::time_point<chrono::system_clock> *&start, chrono::time_point<chrono::system_clock> *&end)
{
	size_t fnSize = 0;
	char *data = nullptr;
	size_t size = 0;
	port->read(data, size, start, end);
	
	if (data == nullptr) {
		return nullptr;
	}
	File *file = parseData(data, size);
	return file;
}

bool ConnectionManager::canRead()
{
	return this->isPortOpen() && this->port->canRead();
}

ConnectionManager::~ConnectionManager()
{
	try {
	// if (this->port) {
	// delete this->port;

	}
	catch (exception e) {
		cout << "Blad podczas zamykania polaczenia\n";
	}

}
