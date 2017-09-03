#include "../models/Port.h"
#include "../models/File.h"
#include <string>
#include "../lib/NullPointerException.h"
#include <thread>
#include <memory>
#include <iostream>
using namespace std;

class ConnectionManager
{
	Port *port;
	size_t sendFileNmae(string fileName);
	size_t sendData(char *data, size_t size);
	string filenameMessage = "filename:";
	File *parseData(char *data, size_t size);
public:
	ConnectionManager();
	bool isPortOpen();
	bool init(string portName);
	size_t sendFile(File *file);
	File *receiveFile();

	~ConnectionManager();
};