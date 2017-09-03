#include "ConnectionManager.h"

size_t ConnectionManager::sendFileNmae(string fileName)
{
	port->setRTS();
	string message = "file:" + fileName + "\n\r\n\rEOF";
	char *data = new char[message.size()];
	strcpy(data, message.c_str());
	size_t bytesWritten = port->write(data, message.size());
	port->clearRTS();
	try {
	delete data;

	}
	catch (exception e) {
		cout << e.what() << endl;
	}

	return bytesWritten;
}

size_t ConnectionManager::sendData(char *data, size_t size)
{
	if (!data) {
		throw new NullPointerException;
	}
	port->setRTS();
	char dataPrefix[] = "data:";
	this->port->write(dataPrefix, sizeof(dataPrefix));
	size_t bytesWritten = this->port->write(data, size);
	port->clearRTS();
	return bytesWritten;
}

File *ConnectionManager::parseData(char *data, size_t size)
{
	cout << "parsing resp\n";
	char namePrefix[] = "file:";
	char namePostFix[] = "\n\r\n\rEOF";
	char dataPrefix[] = "\n\r\n\rEOFdata:";
	size_t sizeOfData = size - sizeof(namePrefix) - sizeof(dataPrefix);
	char *namePointer;
	char *dataPointer;
	char *fileData = new char[size];
	char *endNamePointer;
	namePointer = strstr(data, namePrefix);
	endNamePointer = strstr(data, namePostFix);
	dataPointer = strstr(data, dataPrefix);
	string fileName = "";
	if (namePointer && endNamePointer) {
		while (namePointer != endNamePointer) {
			fileName.append(namePointer);
			++namePointer;
		}
		cout << "obtainted filename: " << fileName << endl;
	} if (dataPointer) {
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

size_t ConnectionManager::sendFile(File *file)
{
	if (!file) {
		throw new NullPointerException();
	}

	this->sendFileNmae(file->getFileName());
	size_t result = this->sendData(file->getData(), file->getSize());
	delete file;
	return result;
}

File *ConnectionManager::receiveFile()
{
	size_t fnSize = 0;
	char *filename = new char[fnSize];
	this->port->rawRead(filename, fnSize);
	cout << "received f " << filename << ", size: " << fnSize << endl;
	char *data = nullptr;
	size_t size = 0;
	while (this->port->getCbInQue() == 0) {
		//        cout << "nic";
	}
	cout << "gotSomething " << this->port->getCbInQue() << endl;
	port->read(data, size);
	return nullptr;
	//    return this->parseData(data, size);
	//    File *file( new File(data, filename, size) );
	//    return file;
}

ConnectionManager::~ConnectionManager()
{
	try {
	// if (this->port) {
	// delete this->port;

	}
	catch (exception e) {
		cout << "exception: " << e.what() << endl;
	}

}
